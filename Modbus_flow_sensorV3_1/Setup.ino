void _setup() {
  Serial.begin(115200);
  EEPROM.begin(128);

  //PIN CONFIGS
  pinMode(ledPin, OUTPUT);
  pinMode(hornPin, OUTPUT);
  pinMode(pulse_out_pin, OUTPUT);

  pinMode(pulse_in_pin, INPUT);
  pinMode(button_in_pin, INPUT_PULLUP);

  //WIFI SETUP
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.print("IP address: ");
  // Serial.print(WiFi.localIP());
  // Serial.print("\nMAC Address: ");
  // Serial.println(WiFi.macAddress());
    // Try to connect to flash stored SSID, start AP if fails after timeout
  IPAddress myIP = server.startWiFi(15000, "ESP_AP", "123456789" );

  // FILESYSTEM INIT
  startFilesystem();

  /*
  * Getting FS info (total and free bytes) is strictly related to
  * filesystem library used (LittleFS, FFat, SPIFFS etc etc) and ESP framework
  */
  #ifdef ESP32
  server.setFsInfoCallback([](fsInfo_t* fsInfo) {
    fsInfo->fsName = "LittleFS";
    fsInfo->totalBytes = LittleFS.totalBytes();
    fsInfo->usedBytes = LittleFS.usedBytes();
  });
  #endif

  // Enable ACE FS file web editor and add FS info callback function
  server.enableFsCodeEditor();

  // Add custom handlers to webserver
  server.on("/led", HTTP_GET, handleLed);
  server.on("/firmware_update", HTTP_GET, handleUpdate);

  // Add handler as lambda function (just to show a different method)
  server.on("/version", HTTP_GET, [](AsyncWebServerRequest *request) {
    server.getOptionValue("New firmware JSON", fimwareInfo);

    EEPROM.get(0, fw_version);
    if (fw_version[0] == 0xFF) // Still not stored in EEPROM (first run)
      strcpy(fw_version, "0.0.0");
    String reply = "{\"version\":\"";
    reply += fw_version;
    reply += "\", \"newFirmwareInfoJSON\":\"";
    reply += fimwareInfo;
    reply += "\"}";
    // Send to client actual firmware version and address where to check if new firmware available
    request->send(200, "text/json", reply);
  });

  // Configure /setup page and start Web Server
  server.addOptionBox("Remote Update");
  server.addOption("New firmware JSON", fimwareInfo);

  // Start server with built-in websocket event handler
  server.init();

  
  // print out docs--------------------------------------------------------------------------------------
  Serial.println("---------------------------------------------------------------------------------------");
  Serial.println("Modbus IP enabled Chlorination Controller V3");
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
    "This is \"remoteOTA.ino\" example.\n"
    "Open /setup page to configure optional parameters.\n"
    "Open /edit page to view, edit or upload example or your custom webserver source files."
  ));

  loadData();
  if (valid_flash != 12345) {
    resetDefaults();
  }

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