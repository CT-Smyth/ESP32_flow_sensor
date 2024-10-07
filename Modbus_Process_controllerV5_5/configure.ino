void config_and_init() {
  Serial.begin(115200);
  EEPROM.begin(128);

  //PIN CONFIGS
  pinMode(ledPin, OUTPUT);
  pinMode(hornPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(pulse_in_pin, INPUT);
  pinMode(button_in_pin, INPUT_PULLUP);

  // Automatically connect using saved credentials,
  WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP

  WPASS = wm.getWiFiPass();
  WSSID = wm.getWiFiSSID();

  if (WSSID != "") {
    Serial.print("\nConnect Wifi to:");
    Serial.print(WSSID);
    Serial.print("  Using:");
    Serial.println(WPASS);
    WiFi.begin(WSSID, WPASS);

  } else {
    Serial.println("No credentials found. Reboot while holding down button to configure");
    boot_mode = 1;
  }

  //MODBUS SETUP
  mBus.onConnect(connect_callback);  // Add callback on connection event. must return 1 to allow connection
  mBus.server();

  //COILs
  mBus.addCoil(HORN_COIL);
  mBus.onSetCoil(HORN_COIL, set_HORN_callback);
  mBus.onGetCoil(HORN_COIL, get_HORN_callback);

  mBus.addCoil(RELAY_COIL);
  mBus.onSetCoil(RELAY_COIL, set_RELAY_callback);
  mBus.onGetCoil(RELAY_COIL, get_RELAY_callback);

  mBus.addCoil(STOP_STATUS_COIL);
  mBus.onSetCoil(STOP_STATUS_COIL, set_STOP_callback);
  mBus.onGetCoil(STOP_STATUS_COIL, get_STOP_callback);

  mBus.addCoil(SAVE_COIL);
  mBus.onSetCoil(SAVE_COIL, set_SAVE_callback);
  mBus.onGetCoil(SAVE_COIL, get_SAVE_callback);

  //ISTS
  mBus.addIsts(HORN_COIL);
  mBus.onSetIsts(HORN_COIL, set_HORN_ISTS_callback);
  mBus.onGetIsts(HORN_COIL, get_HORN_ISTS_callback);

  mBus.addIsts(RELAY_COIL);
  mBus.onSetIsts(RELAY_COIL, set_RELAY_ISTS_callback);
  mBus.onGetIsts(RELAY_COIL, get_RELAY_ISTS_callback);

  //IREGs
  mBus.addIreg(LIFETIME_UNITS_MSW, lifetime_units_MSW);
  mBus.onGetIreg(LIFETIME_UNITS_MSW, get_LIFETIME_UNITS_MSW_callback);

  mBus.addIreg(LIFETIME_UNITS_LSW, lifetime_units_LSW);
  mBus.onGetIreg(LIFETIME_UNITS_LSW, get_LIFETIME_UNITS_LSW_callback);

  mBus.addIreg(THIS_FLOW_UNITS, 0);
  mBus.onGetIreg(THIS_FLOW_UNITS, get_THIS_FLOW_UNITS_callback);

  mBus.addIreg(THIS_FLOW_DURATION, 0);
  mBus.onGetIreg(THIS_FLOW_DURATION, get_THIS_FLOW_DURATION_callback);

  mBus.addIreg(LARGEST_FLOW_UNITS, largest_flow_units);
  mBus.onGetIreg(LARGEST_FLOW_UNITS, get_LARGEST_FLOW_UNITS_callback);

  mBus.addIreg(LONGEST_FLOW_DURATION, longest_flow_duration);
  mBus.onGetIreg(LONGEST_FLOW_DURATION, get_LONGEST_FLOW_DURATION_callback);

  mBus.addIreg(FLOW_RATE, 0);
  mBus.onGetIreg(FLOW_RATE, get_FLOW_RATE_callback);

  mBus.addIreg(OUTPUT_PULSES_TODO, 0);
  mBus.onGetIreg(OUTPUT_PULSES_TODO, get_OUTPUT_PULSES_TODO_callback);

  mBus.addIreg(SENSOR_PULSES, 0);
  mBus.onGetIreg(SENSOR_PULSES, get_SENSOR_PULSES_callback);

  mBus.addIreg(ANALOG_SENSE, 0);  //  Use addIreg() for analog Inputs
  mBus.onGetIreg(ANALOG_SENSE, get_ANALOG_SENSE_callback);

  mBus.addIreg(UNITS_QUEUED, 0);
  mBus.onGetIreg(UNITS_QUEUED, get_UNITS_QUEUED_callback);

  mBus.addIreg(UPTIME_HRS, 0);
  mBus.onGetIreg(UPTIME_HRS, get_UPTIME_HRS_callback);

  mBus.addIreg(WIFI_RSSI, 0);
  mBus.onGetIreg(WIFI_RSSI, get_WIFI_RSSI_callback);


  //HREGs
  mBus.addHreg(OUTPUT_PULSES_TO_ADD, 0);
  mBus.onSetHreg(OUTPUT_PULSES_TO_ADD, set_OUTPUT_PULSES_TO_ADD_callback);
  mBus.onGetHreg(OUTPUT_PULSES_TO_ADD, get_OUTPUT_PULSES_TO_ADD_callback);

  mBus.addHreg(UNITS_SINCE_BOOT, 0);
  mBus.onSetHreg(UNITS_SINCE_BOOT, set_UNITS_SINCE_BOOT_callback);
  mBus.onGetHreg(UNITS_SINCE_BOOT, get_UNITS_SINCE_BOOT_callback);

  mBus.addHreg(FLOW_TIME_SINCE_BOOT, 0);
  mBus.onSetHreg(FLOW_TIME_SINCE_BOOT, set_FLOW_TIME_SINCE_BOOT_callback);
  mBus.onGetHreg(FLOW_TIME_SINCE_BOOT, get_FLOW_TIME_SINCE_BOOT_callback);

  mBus.addHreg(SHOW_DISPLAY, 0);
  mBus.onSetHreg(SHOW_DISPLAY, set_SHOW_DISPLAY_callback);
  mBus.onGetHreg(SHOW_DISPLAY, get_SHOW_DISPLAY_callback);

  mBus.addHreg(MODE, 0);
  mBus.onSetHreg(MODE, set_MODE_callback);
  mBus.onGetHreg(MODE, get_MODE_callback);

  mBus.addHreg(PIN_MODE, 0);
  mBus.onSetHreg(PIN_MODE, set_PIN_MODE_callback);
  mBus.onGetHreg(PIN_MODE, get_PIN_MODE_callback);

  mBus.addHreg(SENSOR_PULSES_PER_UNIT, 0);
  mBus.onSetHreg(SENSOR_PULSES_PER_UNIT, set_SENSOR_PULSES_PER_UNIT_callback);
  mBus.onGetHreg(SENSOR_PULSES_PER_UNIT, get_SENSOR_PULSES_PER_UNIT_callback);

  mBus.addHreg(SAVE_INTERVAL, 0);
  mBus.onSetHreg(SAVE_INTERVAL, set_SAVE_INTERVAL_callback);
  mBus.onGetHreg(SAVE_INTERVAL, get_SAVE_INTERVAL_callback);

  mBus.addHreg(HORN_UNITS, 0);
  mBus.onSetHreg(HORN_UNITS, set_HORN_UNITS_callback);
  mBus.onGetHreg(HORN_UNITS, get_HORN_UNITS_callback);

  mBus.addHreg(RELAY_UNITS, 0);
  mBus.onSetHreg(RELAY_UNITS, set_RELAY_UNITS_callback);
  mBus.onGetHreg(RELAY_UNITS, get_RELAY_UNITS_callback);

  mBus.addHreg(HORN_SECONDS, 0);
  mBus.onSetHreg(HORN_SECONDS, set_HORN_SECONDS_callback);
  mBus.onGetHreg(HORN_SECONDS, get_HORN_SECONDS_callback);

  mBus.addHreg(RELAY_SECONDS, 0);
  mBus.onSetHreg(RELAY_SECONDS, set_RELAY_SECONDS_callback);
  mBus.onGetHreg(RELAY_SECONDS, get_RELAY_SECONDS_callback);

  mBus.addHreg(FLOW_STOP_SECONDS, 0);
  mBus.onSetHreg(FLOW_STOP_SECONDS, set_FLOW_STOP_SECONDS_callback);
  mBus.onGetHreg(FLOW_STOP_SECONDS, get_FLOW_STOP_SECONDS_callback);

  mBus.addHreg(RELAY_LATCHMODE, 0);
  mBus.onSetHreg(RELAY_LATCHMODE, set_RELAY_LATCHMODE_callback);
  mBus.onGetHreg(RELAY_LATCHMODE, get_RELAY_LATCHMODE_callback);

  mBus.addHreg(HORN_LATCHMODE, 0);
  mBus.onSetHreg(HORN_LATCHMODE, set_HORN_LATCHMODE_callback);
  mBus.onGetHreg(HORN_LATCHMODE, get_HORN_LATCHMODE_callback);

  mBus.addHreg(OUTPUT_PULSE_MS, 0);
  mBus.onSetHreg(OUTPUT_PULSE_MS, set_OUTPUT_PULSE_MS_callback);
  mBus.onGetHreg(OUTPUT_PULSE_MS, get_OUTPUT_PULSE_MS_callback);

  mBus.addHreg(OUTPUT_PULSE_OFF_MS, 0);
  mBus.onSetHreg(OUTPUT_PULSE_OFF_MS, set_OUTPUT_PULSE_OFF_MS_callback);
  mBus.onGetHreg(OUTPUT_PULSE_OFF_MS, get_OUTPUT_PULSE_OFF_MS_callback);

  mBus.addHreg(UNITS_PER_EVENT, 0);
  mBus.onSetHreg(UNITS_PER_EVENT, set_UNITS_PER_EVENT_callback);
  mBus.onGetHreg(UNITS_PER_EVENT, get_UNITS_PER_EVENT_callback);

  mBus.addHreg(OUTPUT_PULSES_PER_TRIGGER, 0);
  mBus.onSetHreg(OUTPUT_PULSES_PER_TRIGGER, set_OUTPUT_PULSES_PER_TRIGGER_callback);
  mBus.onGetHreg(OUTPUT_PULSES_PER_TRIGGER, get_OUTPUT_PULSES_PER_TRIGGER_callback);

  mBus.addHreg(OUTPUT_PULSES_FOR_PRIME, 0);
  mBus.onSetHreg(OUTPUT_PULSES_FOR_PRIME, set_OUTPUT_PULSES_FOR_PRIME_callback);
  mBus.onGetHreg(OUTPUT_PULSES_FOR_PRIME, get_OUTPUT_PULSES_FOR_PRIME_callback);

  mBus.addHreg(MIN_FLOW, 0);
  mBus.onSetHreg(MIN_FLOW, set_MIN_FLOW_callback);
  mBus.onGetHreg(MIN_FLOW, get_MIN_FLOW_callback);

  mBus.addHreg(ANALOG_THRESHOLD, 0);
  mBus.onSetHreg(ANALOG_THRESHOLD, set_ANALOG_THRESHOLD_callback);
  mBus.onGetHreg(ANALOG_THRESHOLD, get_ANALOG_THRESHOLD_callback);

  mBus.addHreg(ANALOG_MAP_MIN, 0);
  mBus.onSetHreg(ANALOG_MAP_MIN, set_ANALOG_MAP_MIN_callback);
  mBus.onGetHreg(ANALOG_MAP_MIN, get_ANALOG_MAP_MIN_callback);

  mBus.addHreg(ANALOG_MAP_MAX, 0);
  mBus.onSetHreg(ANALOG_MAP_MAX, set_ANALOG_MAP_MAX_callback);
  mBus.onGetHreg(ANALOG_MAP_MAX, get_ANALOG_MAP_MAX_callback);

  mBus.addHreg(ANALOG_RANGE_MIN, 0);
  mBus.onSetHreg(ANALOG_RANGE_MIN, set_ANALOG_RANGE_MIN_callback);
  mBus.onGetHreg(ANALOG_RANGE_MIN, get_ANALOG_RANGE_MIN_callback);

  mBus.addHreg(ANALOG_RANGE_MAX, 0);
  mBus.onSetHreg(ANALOG_RANGE_MAX, set_ANALOG_RANGE_MAX_callback);
  mBus.onGetHreg(ANALOG_RANGE_MAX, get_ANALOG_RANGE_MAX_callback);

  mBus.addHreg(FLOWRATE_SECONDS, 0);
  mBus.onSetHreg(FLOWRATE_SECONDS, set_FLOWRATE_SECONDS_callback);
  mBus.onGetHreg(FLOWRATE_SECONDS, get_FLOWRATE_SECONDS_callback);

  mBus.addHreg(ADD_TOTAL_UNITS, 0);
  mBus.onSetHreg(ADD_TOTAL_UNITS, set_ADD_TOTAL_UNITS_callback);
  mBus.onGetHreg(ADD_TOTAL_UNITS, get_ADD_TOTAL_UNITS_callback);

  mBus.addHreg(RESET_HREG);
  mBus.onSetHreg(RESET_HREG, set_RESET_callback);
  mBus.onGetHreg(RESET_HREG, get_RESET_callback);


  //const char *HOST_NAME = "flowmeter";  //set hostname to <hostname>.local
  const char *HOST_NAME = WiFi.getHostname();

#ifdef MDNS_RESPONDER
  if (!MDNS.begin(HOST_NAME)) {  // Set the hostname to "esp32.local"
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
#endif


#ifdef STATIC_IP
  // Configuring static IP
  if (!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure Static IP");
  }
#endif

  ten_ms_heartbeat = millis();
  buttonTime = millis();
  rawPulseTime = millis();
  mBus.task();

// ESP32 Watchdog timer -    Note: esp32 board manager v3.x.x requires different code
#define WDT_TIMEOUT 30

#if defined ESP32
  esp_task_wdt_deinit();  // ensure a watchdog is not already configured
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR == 3
    // v3 board manager detected
    // Create and initialize the watchdog timer(WDT) configuration structure
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = WDT_TIMEOUT * 1000,  // Convert seconds to milliseconds
    .idle_core_mask = 1 << 0,          // Monitor core 1 only
    .trigger_panic = true              // Enable panic
  };
  // Initialize the WDT with the configuration structure
  esp_task_wdt_init(&wdt_config);  // Pass the pointer to the configuration structure
  esp_task_wdt_add(NULL);          // Add current thread to WDT watch
  esp_task_wdt_reset();            // reset timer
#else
    // pre v3 board manager assumed
  esp_task_wdt_init(WDT_TIMEOUT, true);  //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);                //add current thread to WDT watch
#endif
#endif

  manPage();

  // init display
  // #ifdef MY_OLED
  // obdI2CInit(&oled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L);
  // obdFill(&oled, OBD_WHITE, 1);  // clear screen
  // #endif

  //restore state from flash
  loadData();
  loadSettings();
  restoreOutputs();
  syncModbusData();
  syncModbusSettings();
  loadBootMode();
  display_mode = 2;

  Serial.print("BOOT MODE: ");
  Serial.println(boot_mode);

  system_initialized = true;

  if (valid_flash != 12345) {
    resetDefaults();
  }

#ifdef MY_OLED
  obdI2CInit(&oled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L);
#endif

#ifdef MY_OLED
  obdFill(&oled, OBD_WHITE, 1);
#ifdef C3_42_OLED
  obdWriteString(&oled, 0, 30, 16, (char *)"  v5.5", FONT_8x8, OBD_BLACK, 1);
  obdWriteString(&oled, 0, 30, 24, (char *)"HOLD BUTTON", FONT_6x8, OBD_BLACK, 1);
  obdWriteString(&oled, 0, 30, 32, (char *)"TO SET WIFI", FONT_6x8, OBD_BLACK, 1);
  obdWriteString(&oled, 0, 30, 40, (char *)"OR OTA UPDT", FONT_6x8, OBD_BLACK, 1);
#else
  obdWriteString(&oled, 0, 0, 0, (char *)"  v5.5", FONT_8x8, OBD_BLACK, 1);
  obdWriteString(&oled, 0, 0, 8, (char *)" HOLD BUTTON", FONT_6x8, OBD_BLACK, 1);
  obdWriteString(&oled, 0, 0, 24, (char *)" TO SET WIFI", FONT_6x8, OBD_BLACK, 1);
  obdWriteString(&oled, 0, 0, 40, (char *)"OR OTA UPDATE", FONT_6x8, OBD_BLACK, 1);
#endif

#endif

  delay(2000);
  int debounce = 0;

  for (int t = 0; t < 11; t++) {
    if (digitalRead(button_in_pin) == 0) {
      debounce++;
    }
  }

  //FINISH STARTUP
  Serial.print("\nSTARTING UP ..");
  digitalWrite(ledPin, ledOn);
  delay(50);
  Serial.print("..");
  digitalWrite(ledPin, !ledOn);
  delay(50);
  Serial.print("..");
  digitalWrite(ledPin, ledOn);
  delay(50);
  Serial.print("..");
  digitalWrite(ledPin, !ledOn);


  if ((debounce > 8) || (boot_mode == 1)) {
    boot_mode = 0;
    saveBootMode();
    char szTemp[32];
    uint8_t IP2 = WiFi.localIP()[2];
    uint8_t IP3 = WiFi.localIP()[3];
    
#ifdef MY_OLED
obdFill(&oled, OBD_WHITE, 1);
#ifdef C3_42_OLED
    obdWriteString(&oled, 0, 30, 16, (char *)" OTA and ", FONT_6x8, OBD_BLACK, 1);
    obdWriteString(&oled, 0, 30, 24, (char *)"WiFI config", FONT_6x8, OBD_BLACK, 1);
    sprintf(szTemp, "x.x.%d.%d", IP2, IP3);
    obdWriteString(&oled, 0, 30, 32, szTemp, FONT_6x8, OBD_BLACK, 1);
    obdWriteString(&oled, 0, 30, 40, (char *)"192.168.4.1", FONT_6x8, OBD_BLACK, 1);
#else
    obdWriteString(&oled, 0, 0, 0, (char *)" OTA and ", FONT_6x8, OBD_BLACK, 1);
    obdWriteString(&oled, 0, 0, 8, (char *)"WiFI config", FONT_6x8, OBD_BLACK, 1);
    sprintf(szTemp, "x.x.%d.%d", IP2, IP3);
    obdWriteString(&oled, 0, 5, 24, szTemp, FONT_6x8, OBD_BLACK, 1);
    obdWriteString(&oled, 0, 0, 32, (char *)"192.168.4.1", FONT_6x8, OBD_BLACK, 1);
#endif
#endif

    digitalWrite(ledPin, ledOn);
    delay(150);
    digitalWrite(ledPin, !ledOn);
    delay(150);
    digitalWrite(ledPin, ledOn);
    delay(150);
    digitalWrite(ledPin, !ledOn);
    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(300);
    wm.startConfigPortal();
    delay(10000);
  }

}