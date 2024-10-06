//----------------------------------TODO-------------------------------------------
//wifi signal strength available over modbus ?? WiFi.RSSI()

//more testing --- test flash storage works
//FIX address for update server
//Allow setting of calibration variables in the webserver
//serial bluetooth? https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/
//make BT name unuiqe? use ChipID?  Make it settable in preferences using pushbutton to enter a config mode?

//modify for general use (water meter, alarm, shutoff)
//version for wemos S2 mini? Choose board LOLIN S2 MINI or LOLIN S2 PICO
//version for wemos s3 mini? Choose board LOLIN S3


//---------------------------------------------------------------------------------


//Listens for modbus on port 502 - Slave ID not important

//COILs
// HORN_COIL = 0;   //(1) ---Alarm control
// STOP_COIL = 1;   //(2) ---Stops current job
// SAVE_COIL = 2;   //(3) ---Saves current data
// RESET_COIL = 9;  //(10) ---resets to defaults

//IREGs
// LIFETIME_UNITS_MSW = 0;  //(1)  (READ) ---upper 16 bits of total lifetime output pulses
// LIFETIME_UNITS_LSW = 1;  //(2)  (READ) ---lower 16 bits of total lifetime output pulses
// OUTPUT_PULSES_TODO = 2;  //(3) (READ)  ---Number of output pulses pending in current job
// (inactive) SENSOR_IREG = 3; (4) ---unused Analog input


//HREGs
// OUTPUT_PULSES_TO_ADD = 0;  //(1) ---put pulses to add to current job - erased after use

// -------------These settings will not be saved unless SAVE_COIL is toggled----------------
// OUTPUT_PULSE_MS_CAL = 9;    //(10) ---milliseconds for pulse length (off time will be 2x)
// PULSES_PER_UNIT_CAL = 10;   //(11) ---Input pulses for each output pulse
// PULSES_FOR_PRIME_CAL = 11;  //(12) ---Pulses to add to current job for pump priming
// SAVE_INTERVAL_CAL = 12;     //(13) ---Save interval. How many pulses between saves to flash


#include <WiFi.h> //builtin
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <Preferences.h> //included
#include <ModbusIP_ESP8266.h> //4.10 https://github.com/emelianov/modbus-esp8266
//#include <WiFiClientSecure.h> //builtin
//#include <HTTPClient.h> //builtin
//#include <HTTPUpdate.h> //builtin
#include <EEPROM.h>  //builtin For storing the firmware version
#include <FS.h> //builtin
//#include <LittleFS.h>
//#include <AsyncFsWebServer.h>  //1.07 https://github.com/cotestatnt/async-esp-fs-webserver/



#define _SENSOR_PULSES "Sensor pulses since last unit"
#define _LIFETIME_UNITS_MSW "Logged Units x 65336 (IREG0)"
#define _LIFETIME_UNITS_LSW "Logged Units (IREG1)"
#define _PULSES_TODO "Output Pulse Cycles TODO (IREG2)"

#define _PULSE_MS "Output pulse length in mS (HREG9)"
#define _PULSES_PER_UNIT "Input pulses per unit (HREG10)"
#define _PULSES_FOR_PRIME "Priming Pulses (HREG11)"
#define _SAVE_INTERVAL "Units for save interval (HREG12)"

Preferences keystore;

//#define FILESYSTEM LittleFS
//AsyncFsWebServer server(80, FILESYSTEM);

//Add your wifi network's credentials here
const char* ssid = "BELLAVISTA";
const char* password = "Brb19icdA!";
//const char* ssid     = "CONTROL";
//const char* password = "CNTRL2967!";

//Modbus Registers Offsets

const int HORN_COIL = 0;   //(1)
const int STOP_COIL = 1;   //(2)
const int SAVE_COIL = 2;   //(3)
const int RESET_COIL = 9;  //(10)

const int LIFETIME_UNITS_MSW = 0;  //(1) IREG
const int LIFETIME_UNITS_LSW = 1;  //(2) IREG
const int OUTPUT_PULSES_TODO = 2;  //(1) IREG
const int SENSOR_IREG = 3;         //(4) IREG

const int OUTPUT_PULSES_TO_ADD = 0;   //(1) HREG
const int OUTPUT_PULSE_MS_CAL = 9;    //(10) HREG
const int PULSES_PER_UNIT_CAL = 10;   //(11) HREG
const int PULSES_FOR_PRIME_CAL = 11;  //(12) HREG
const int SAVE_INTERVAL_CAL = 12;     //(13) HREG


//Pins
const int pulse_in_pin = 18;
const int button_in_pin = 4;

const int ledPin = LED_BUILTIN;
const int hornPin = 17;
//const int pulse_out_pin = 17;
const int pulse_out_pin = 33;

long button_time;
long PulseInTime;
long ModbusTime;
long PulseTime;

//these values loaded from flash
uint16_t valid_flash;

uint16_t lifetime_units_from_flash_MSW;
uint16_t lifetime_units_from_flash_LSW;

uint16_t output_pulse_ms;
uint16_t sensor_pulses_per_unit;
uint16_t output_pulses_for_prime;
uint16_t new_units_save_interval;
//end values loaded from flash

uint16_t sensor_pulses;
uint16_t lifetime_units_MSW;
uint16_t lifetime_units_LSW;
uint16_t output_pulses_todo;

int buttonSeconds;

bool pulse_on_in_progress = false;
bool pulse_off_in_progress = false;
bool call_for_pulse = false;
bool last_pulse_pin_state;
bool captiveRun = false;

//ModbusIP object
ModbusIP mBus;
//uint8_t New_MAC_Address[] = {0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x01};

void setup() {
  _setup();
}

//-----------------------------------------------------------------------------------------------
void loop() {
  //esp_task_wdt_reset();

  if (millis() > PulseInTime) {  // check pulses every 1ms
    PulseInTime = millis();
    if (checkPulse() > 0) {  //check for rising edge on pulse input
      sensor_pulses++;
      if (sensor_pulses >= sensor_pulses_per_unit) {
        output_pulses_todo++;
        sensor_pulses = sensor_pulses - sensor_pulses_per_unit;
      }
      //Serial.print("pulses : ");
      //Serial.println(sensor_pulses);
    }
  }

  // Call the modbus task every 100ms
  if (millis() > ModbusTime + 100) {
    ModbusTime = millis();
    handleModbus();
    buttonSeconds = checkButton();
    if (buttonSeconds == 1) {
      Serial.println("cancel job in progress");
      stop();
    }
    if (buttonSeconds == 5) {
      Serial.println("Priming....");
      output_pulses_todo = 100;
    }
    mBus.Ireg(SENSOR_IREG, analogRead(A0));  // read analog A0
  }



  output_pulser();
}
//-----------------------------------------------------------------------------------------------



uint checkPulse() {
  bool pulse_pin_state;
  pulse_pin_state = digitalRead(pulse_in_pin);
  digitalWrite(ledPin, pulse_pin_state);
  if (last_pulse_pin_state != pulse_pin_state) {
    last_pulse_pin_state = pulse_pin_state;
    return (pulse_pin_state);  // 1 for changed to true, 0 if changed to false
  } else {
    return (0);  //if unchanged
  }
}


int checkButton() {
  int debounce = 0;

  for (int t = 0; t < 11; t++) {
    if (digitalRead(button_in_pin) == 0) debounce++;
  }

  if (debounce > 8) {
    if (button_time == 0) button_time = millis();
    //Serial.println(buttonSeconds);
  } else {
    button_time = 0;
    button_time = millis();  //if this is not here you get a false trigger when millis is 5000 lol
  }

  return (int((millis() - button_time) / 1000));
}


uint output_pulser() {
  if (output_pulses_todo > 0) {
    //Serial.print("pulses to do : ");
    //Serial.println(output_pulses_todo);

    if ((pulse_on_in_progress == false) && (pulse_off_in_progress == false)) {  //start pulse
      digitalWrite(pulse_out_pin, 1);
      //Serial.println("Start pulse on");
      pulse_on_in_progress = true;
      pulse_off_in_progress = false;
      PulseTime = millis() + output_pulse_ms;
    }

    if ((pulse_on_in_progress == true) && (millis() > PulseTime)) {  //end 'on' segment of pulse
      digitalWrite(pulse_out_pin, 0);
      //Serial.println("Start pulse off");
      pulse_on_in_progress = false;
      pulse_off_in_progress = true;
      PulseTime = millis() + (output_pulse_ms * 2);  //2x for 'off' pulse segment
    }

    if ((pulse_off_in_progress == true) && (millis() > PulseTime)) {  //end 'on' segment of pulse
      digitalWrite(pulse_out_pin, 0);
      //Serial.println("End pulse");
      pulse_on_in_progress = false;
      pulse_off_in_progress = false;
      output_pulses_todo = output_pulses_todo - 1;
      lifetime_units_LSW++;
      if (lifetime_units_LSW == 0) {
        lifetime_units_MSW++;
      }


      Serial.print("\npulses to do : ");
      Serial.print(output_pulses_todo);
      Serial.print("----------------lifetime units : ");
      Serial.print(lifetime_units_MSW);
      Serial.print(":");
      Serial.println(lifetime_units_LSW);

      if ((lifetime_units_LSW - lifetime_units_from_flash_LSW) >= new_units_save_interval) {
        saveData();
      }
    }
  } else {
    stop();
  }
  return output_pulses_todo;
}


void stop() {
  output_pulses_todo = 0;
  pulse_on_in_progress = false;
  pulse_off_in_progress = false;
  digitalWrite(pulse_out_pin, 0);
  //PulseTime = millis()
}
