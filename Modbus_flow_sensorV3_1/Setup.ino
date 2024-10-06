void _setup() {
  Serial.begin(115200);
  EEPROM.begin(128);

  //PIN CONFIGS
  pinMode(ledPin, OUTPUT);
  pinMode(hornPin, OUTPUT);
  pinMode(pulse_out_pin, OUTPUT);

  pinMode(pulse_in_pin, INPUT);
  pinMode(button_in_pin, INPUT_PULLUP);


  // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // put your setup code here, to run once:
  Serial.begin(115200);

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "1qazXSW@");  // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  Serial.print("\nDefault ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());

  loadData();
  if (valid_flash != 12345) {
    resetDefaults();
  }

  // print out docs--------------------------------------------------------------------------------------
  Serial.println("---------------------------------------------------------------------------------------");
  Serial.println("Modbus IP enabled Process Controller V3");
  Serial.println("---------------------------------------------------------------------------------------");
  Serial.println("\nListens for modbus on port 502 - Slave ID not important");
  Serial.println("\n\nCOILs:");
  Serial.println("HORN_COIL  = 0 (1) ---Alarm control");
  Serial.println("STOP_COIL  = 1 (2) ---Stops current job");
  Serial.println("SAVE_COIL  = 2 (3) ---Saves current data");
  Serial.println("RESET_COIL = 9 (10) ---resets to defaults");
  Serial.println("\nIREGs:");
  Serial.println("LIFETIME_UNITS_MSW = 0 (1) ---upper 16 bits of total lifetime output pulses");
  Serial.println("LIFETIME_UNITS_LSW = 1 (2) ---lower 16 bits of total lifetime output pulses");
  Serial.println("OUTPUT_PULSES_TODO = 2 (3) ---Number of output pulses pending in current job");
  Serial.println("SENSOR_IREG        = 3 (4) ---Analog input on A0 0-1v ");

  Serial.println("\nHREGs:");
  Serial.println("OUTPUT_PULSES_TO_ADD = 0 (1)  ---put pulses to add to current job - erased after use");

  Serial.println("\n-----These settings will not be saved unless SAVE_COIL is toggled------");
  Serial.println("OUTPUT_PULSE_MS_CAL  = 9  (10) ---milliseconds for pulse length (off time will be 2x)");
  Serial.println("PULSES_PER_UNIT_CAL  = 10 (11) ---Input pulses for each output pulse");
  Serial.println("PULSES_FOR_PRIME_CAL = 11 (12) ---Pulses to add to current job for pump priming");
  Serial.println("SAVE_INTERVAL_CAL    = 12 (13) ---Save interval. pulses between saves to flash");
  Serial.println("---------------------------------------------------------------------------------------\n\n");
  Serial.print(F("ESP Web Server started on IP Address: "));
  Serial.println(WiFi.localIP());
  Serial.println(F(
    "Open /setup page to configure.\n"
    "Open /edit page to view, edit or upload example or your custom webserver source files."));



  //MODBUS SETUP
  mBus.server();
  mBus.addCoil(HORN_COIL);
  mBus.addCoil(STOP_COIL);
  mBus.addCoil(RESET_COIL);

  mBus.addIreg(OUTPUT_PULSES_TODO, 0);
  mBus.addIreg(LIFETIME_UNITS_MSW, lifetime_units_MSW);
  mBus.addIreg(LIFETIME_UNITS_LSW, lifetime_units_LSW);
  mBus.addIreg(SENSOR_IREG);  //  Use addIreg() for analog Inputs

  mBus.addHreg(OUTPUT_PULSES_TO_ADD, 0);
  mBus.addHreg(PULSES_PER_UNIT_CAL, sensor_pulses_per_unit);
  mBus.addHreg(OUTPUT_PULSE_MS_CAL, output_pulse_ms);
  mBus.addHreg(PULSES_FOR_PRIME_CAL, output_pulses_for_prime);
  mBus.addHreg(SAVE_INTERVAL_CAL, new_units_save_interval);



  digitalWrite(ledPin, 1);
  delay(200);
  digitalWrite(ledPin, 0);
  delay(200);
  digitalWrite(ledPin, 1);
  delay(200);
  digitalWrite(ledPin, 0);
  delay(200);

  ModbusTime = millis();
  button_time = millis();
  handleModbus();
}