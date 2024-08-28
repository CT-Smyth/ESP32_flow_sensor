void handleModbus() {
  mBus.Ireg(LIFETIME_UNITS_MSW, lifetime_units_MSW);
  mBus.Ireg(LIFETIME_UNITS_LSW, lifetime_units_LSW);
  mBus.Ireg(OUTPUT_PULSES_TODO, output_pulses_todo);

  mBus.task();

  if (mBus.Hreg(OUTPUT_PULSES_TO_ADD) > 0) {
    output_pulses_todo = output_pulses_todo + mBus.Hreg(OUTPUT_PULSES_TO_ADD);
    mBus.Hreg(OUTPUT_PULSES_TO_ADD, 0);
  }

  if (mBus.Hreg(OUTPUT_PULSE_MS_CAL) != output_pulse_ms) {
    output_pulse_ms = mBus.Hreg(OUTPUT_PULSE_MS_CAL);
    Serial.println("1toggle coil 2 (3) to save settings");
  }

  if (mBus.Hreg(PULSES_PER_UNIT_CAL) != sensor_pulses_per_unit) {
    sensor_pulses_per_unit = mBus.Hreg(PULSES_PER_UNIT_CAL);
    Serial.println("2toggle coil 2 (3) to save settings");
  }

  if (mBus.Hreg(PULSES_FOR_PRIME_CAL) != output_pulses_for_prime) {
    output_pulses_for_prime = mBus.Hreg(PULSES_FOR_PRIME_CAL);
    Serial.println("3toggle coil 2 (3) to save settings");
  }


  if (mBus.Hreg(SAVE_INTERVAL_CAL) != new_units_save_interval) {
    new_units_save_interval = mBus.Hreg(SAVE_INTERVAL_CAL);
    Serial.println("4toggle coil 2 (3) to save settings");
  }

  if (mBus.Coil(STOP_COIL) == 1) {
    mBus.Coil(STOP_COIL, 0);
    Serial.println("STOP OPERATION");
    stop();
  }

  if (mBus.Coil(SAVE_COIL) == 1) {
    mBus.Coil(SAVE_COIL, 0);
    saveSettings();
  }

  if (mBus.Coil(RESET_COIL) == 1) {
    Serial.println("RESET TO DEFAULTS");
    mBus.Coil(RESET_COIL, 0);
    resetDefaults();
  }


  digitalWrite(hornPin, mBus.Coil(HORN_COIL));
}

void resetDefaults() {  //reset to factory defaults
  Serial.println("Loading defaults");
  valid_flash = 12345;
  output_pulse_ms = 200;
  sensor_pulses_per_unit = 9108;
  //sensor_pulses_per_unit = 10; //TODO testing only
  output_pulses_for_prime = 100;
  lifetime_units_from_flash_MSW = 0;
  lifetime_units_from_flash_LSW = 0;
  new_units_save_interval = 20;
  lifetime_units_MSW = 0;
  lifetime_units_LSW = 0;
  saveData();
  saveSettings();
}

void saveData() {
  Serial.println("\nSAVE DATA");

  keystore.begin("keyData", false);
  keystore.putUShort("units_MSW", lifetime_units_MSW);
  keystore.putUShort("units_LSW", lifetime_units_LSW);
  keystore.end();

  lifetime_units_from_flash_MSW = lifetime_units_MSW;
  lifetime_units_from_flash_LSW = lifetime_units_LSW;

  Serial.print("\n----------------lifetime units : ");
  Serial.print(lifetime_units_MSW);
  Serial.print(":");
  Serial.println(lifetime_units_LSW);
}

void saveSettings() {
  Serial.println("\nSAVE SETTINGS");
  valid_flash = 12345;
  keystore.begin("keyData", false);
  keystore.putUShort("valid_flash", valid_flash);
  keystore.putUShort("output_p_ms", output_pulse_ms);
  keystore.putUShort("p_per_unit", sensor_pulses_per_unit);
  keystore.putUShort("p_for_prime", output_pulses_for_prime);
  keystore.putUShort("save_interval", new_units_save_interval);
  keystore.end();
  
  mBus.Hreg(OUTPUT_PULSE_MS_CAL, output_pulse_ms);
  mBus.Hreg(PULSES_PER_UNIT_CAL, sensor_pulses_per_unit);
  mBus.Hreg(PULSES_FOR_PRIME_CAL, output_pulses_for_prime);
  mBus.Hreg(SAVE_INTERVAL_CAL, new_units_save_interval);

  Serial.print("\n\nvalid_flash: ");
  Serial.print(valid_flash);
  Serial.print("\noutput_pulse_ms: ");
  Serial.print(output_pulse_ms);
  Serial.print("\nsensor_pulses_per_unit: ");
  Serial.print(sensor_pulses_per_unit);
  Serial.print("\noutput_pulses_for_prime: ");
  Serial.print(output_pulses_for_prime);
  Serial.print("\nnew_units_save_interval: ");
  Serial.print(new_units_save_interval);
}

void loadData() {
  Serial.println("\nLOAD DATA");

  keystore.begin("keyData", false);
  valid_flash = keystore.getUShort("valid_flash", 0);
  output_pulse_ms = keystore.getUShort("output_p_ms", 0);
  sensor_pulses_per_unit = keystore.getUShort("p_per_unit", 0);
  output_pulses_for_prime = keystore.getUShort("p_for_prime", 0);
  new_units_save_interval = keystore.getUShort("save_interval", 0);

  lifetime_units_from_flash_MSW = keystore.getUShort("units_MSW", 0);
  lifetime_units_from_flash_LSW = keystore.getUShort("units_LSW", 0);
  keystore.end();

  mBus.Hreg(OUTPUT_PULSE_MS_CAL, output_pulse_ms);
  mBus.Hreg(PULSES_PER_UNIT_CAL, sensor_pulses_per_unit);
  mBus.Hreg(PULSES_FOR_PRIME_CAL, output_pulses_for_prime);
  mBus.Hreg(SAVE_INTERVAL_CAL, new_units_save_interval);
}




//---------------------------modbus reference:----------------------------
// Add local register

// bool addHreg(uint16_t offset, uint16_t value = 0, uint16_t numregs = 1);
// bool addCoil(uint16_t offset, bool value = false, uint16_t numregs = 1);
// bool addIsts(uint16_t offset, bool value = false, uint16_t numregs = 1);
// bool addIreg(uint16_t offset, uint16_t value = 0, uint16_t nemregs = 1);

//     -offset- Address of the first register to add
//     -value- Initial value to be assigned to register(s)
//     -numregs- Count of registers to be created

// Adding new register(s) and assigning value(s). If [some] registers already exists value will be updated. Returns true on success. false if operation is failed for some reason.
// Write local reg

// bool Hreg(uint16_t offset, uint16_t value);
// bool Coil(uint16_t offset, bool value);
// bool Ists(uint16_t offset, bool value);
// bool Ireg(uint16_t offset, uint16_t value);

//     -offset- Address of the register
//     -value- Value to be assigned to register

// Returns true on success. false if register not previousely added or other error.
// Read local reg

// uint16_t Hreg(uint16_t offset);
// bool Coil(uint16_t offset);
// bool Ists(uint16_t offset);
// uint16_t Ireg(uint16_t offset);

//     -offset- Address of the register to read