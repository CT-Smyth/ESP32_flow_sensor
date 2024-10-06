//Modbus callbacks and functionslifetime

void syncModbusData() {
  uint16_t uptime = millis() / 3600000;  //uptime hours
  uint16_t rssi_db = abs(WiFi.RSSI());
  mBus.Ireg(LIFETIME_UNITS_LSW, lifetime_units_LSW);
  mBus.Ireg(LIFETIME_UNITS_MSW, lifetime_units_MSW);
  mBus.Ireg(THIS_FLOW_UNITS, this_flow_units);
  mBus.Ireg(THIS_FLOW_DURATION, this_flow_duration);
  mBus.Ireg(LARGEST_FLOW_UNITS, largest_flow_units);
  mBus.Ireg(LONGEST_FLOW_DURATION, longest_flow_duration);
  mBus.Ireg(FLOW_RATE, flow_rate);
  mBus.Ireg(OUTPUT_PULSES_TODO, output_pulses_todo);
  mBus.Ireg(SENSOR_PULSES, sensor_pulses);
  mBus.Ireg(ANALOG_SENSE, analog_sense);
  mBus.Ireg(UNITS_QUEUED, units_queued);
  mBus.Hreg(UNITS_SINCE_BOOT, units_since_boot);
  mBus.Hreg(FLOW_TIME_SINCE_BOOT, flow_time_since_boot);
  mBus.Hreg(UPTIME_HRS, uptime);
  mBus.Hreg(WIFI_RSSI, rssi_db);

}

void syncModbusSettings() {
  mBus.Hreg(MODE, mode);
  mBus.Hreg(PIN_MODE, pin_mode);
  mBus.Hreg(SENSOR_PULSES_PER_UNIT, sensor_pulses_per_unit);
  mBus.Hreg(SAVE_INTERVAL, save_interval);
  mBus.Hreg(HORN_UNITS, horn_units);
  mBus.Hreg(RELAY_UNITS, relay_units);
  mBus.Hreg(HORN_SECONDS, horn_seconds);
  mBus.Hreg(RELAY_SECONDS, relay_seconds);
  mBus.Hreg(FLOW_STOP_SECONDS, flow_stop_seconds);
  mBus.Hreg(RELAY_LATCHMODE, relay_latchmode);
  mBus.Hreg(HORN_LATCHMODE, horn_latchmode);
  mBus.Hreg(OUTPUT_PULSE_MS, output_pulse_ms);
  mBus.Hreg(OUTPUT_PULSE_OFF_MS, output_pulse_off_ms);
  mBus.Hreg(UNITS_PER_EVENT, units_per_event);
  mBus.Hreg(OUTPUT_PULSES_PER_TRIGGER, output_pulses_per_trigger);
  mBus.Hreg(OUTPUT_PULSES_FOR_PRIME, output_pulses_for_prime);
  mBus.Hreg(MIN_FLOW, min_flow);
  mBus.Hreg(ANALOG_THRESHOLD, analog_threshold);
  mBus.Hreg(ANALOG_MAP_MIN, analog_map_min);
  mBus.Hreg(ANALOG_MAP_MAX, analog_map_max);
  mBus.Hreg(ANALOG_RANGE_MIN, analog_range_min);
  mBus.Hreg(ANALOG_RANGE_MAX, analog_range_max);
  mBus.Hreg(FLOWRATE_SECONDS, flowRate_seconds);
}

bool connect_callback(IPAddress ip) {
  Serial.print("Modbus connected from: ");
  Serial.println(ip);
  if (display_mode == 2) {
    if ((pin_mode == 6) || (pin_mode == 7)) {
      display_mode = 3;
    } else {
      display_mode = 0;
    }
  }
  return true;
}

//coil callbacks
uint16_t set_HORN_callback(TRegister* reg, uint16_t val) {
  horn_set_by_modbus = bool(val);
  setHorn(horn_set_by_modbus);
  return val;
}
uint16_t get_HORN_callback(TRegister* reg, uint16_t val) {
  //dataPage();
  return val;
}

uint16_t set_RELAY_callback(TRegister* reg, uint16_t val) {
  relay_set_by_modbus = bool(val);
  setRelay(relay_set_by_modbus);
  return val;
}
uint16_t get_RELAY_callback(TRegister* reg, uint16_t val) {
  //manPage();
  return val;
}

// uint16_t set_LATCH_RESET_callback(TRegister* reg, uint16_t val) {
//   latchReset();
//   return val;
// }
// uint16_t get_LATCH_RESET_callback(TRegister* reg, uint16_t val) {
//   //Serial.print("value = ");
//   //Serial.println(val);
//   return val;
// }

uint16_t set_STOP_callback(TRegister* reg, uint16_t val) {
  Serial.println("STOP OPERATION");
  stop();
  return val;
}
uint16_t get_STOP_callback(TRegister* reg, uint16_t val) {
  Serial.println("\n\nStatus to serial terminal: \n");
  manPage();
  return val;
}

uint16_t set_SAVE_callback(TRegister* reg, uint16_t val) {
  saveSettings();
  //saveData();
  return val;
}
uint16_t get_SAVE_callback(TRegister* reg, uint16_t val) {
  saveData();
  return val;
}



//Ists callbacks
uint16_t get_HORN_ISTS_callback(TRegister* reg, uint16_t val) {
  return val;
}
uint16_t set_HORN_ISTS_callback(TRegister* reg, uint16_t val) {
  return val;
}

uint16_t get_RELAY_ISTS_callback(TRegister* reg, uint16_t val) {
  return val;
}
uint16_t set_RELAY_ISTS_callback(TRegister* reg, uint16_t val) {
  return val;
}



//Ireg callbacks
uint16_t get_LIFETIME_UNITS_MSW_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_LIFETIME_UNITS_LSW_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_THIS_FLOW_UNITS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_THIS_FLOW_DURATION_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_LARGEST_FLOW_UNITS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_LONGEST_FLOW_DURATION_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_FLOW_RATE_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_OUTPUT_PULSES_TODO_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_SENSOR_PULSES_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_ANALOG_SENSE_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_UNITS_QUEUED_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_UPTIME_HRS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t get_WIFI_RSSI_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


//Hreg Callbacks
uint16_t set_OUTPUT_PULSES_TO_ADD_callback(TRegister* reg, uint16_t val) {
  output_pulses_todo = output_pulses_todo + val;
  Serial.print(val);
  Serial.print(" pulses added ... ");
  Serial.print("total pulses to do = ");
  Serial.println(output_pulses_todo);
  return val;
}
uint16_t get_OUTPUT_PULSES_TO_ADD_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_UNITS_SINCE_BOOT_callback(TRegister* reg, uint16_t val) {
  units_since_boot = val;
  return val;
}
uint16_t get_UNITS_SINCE_BOOT_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_FLOW_TIME_SINCE_BOOT_callback(TRegister* reg, uint16_t val) {
  flow_time_since_boot = val;
  return val;
}
uint16_t get_FLOW_TIME_SINCE_BOOT_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t set_SHOW_DISPLAY_callback(TRegister* reg, uint16_t val) {
  if (millis() < displayTime) {
    display_mode++;
  }
  if (display_mode > 4) {
    display_mode = 0;
  }
  displayTime = millis() + u_long(val * 1000);
  return val;
}
uint16_t get_SHOW_DISPLAY_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t set_MODE_callback(TRegister* reg, uint16_t val) {
  mode = val;
  saveSettings();
  return val;
}
uint16_t get_MODE_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_PIN_MODE_callback(TRegister* reg, uint16_t val) {
  pin_mode = val;
  configure_interrupt_type();
  saveSettings();
  return val;
}
uint16_t get_PIN_MODE_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_SENSOR_PULSES_PER_UNIT_callback(TRegister* reg, uint16_t val) {
  sensor_pulses_per_unit = val;
  saveSettings();
  return val;
}
uint16_t get_SENSOR_PULSES_PER_UNIT_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_SAVE_INTERVAL_callback(TRegister* reg, uint16_t val) {
  save_interval = val;
  saveSettings();
  return val;
}
uint16_t get_SAVE_INTERVAL_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_HORN_UNITS_callback(TRegister* reg, uint16_t val) {
  horn_units = val;
  saveSettings();
  return val;
}
uint16_t get_HORN_UNITS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_RELAY_UNITS_callback(TRegister* reg, uint16_t val) {
  relay_units = val;
  saveSettings();
  return val;
}
uint16_t get_RELAY_UNITS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_HORN_SECONDS_callback(TRegister* reg, uint16_t val) {
  horn_seconds = val;
  saveSettings();
  return val;
}
uint16_t get_HORN_SECONDS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_RELAY_SECONDS_callback(TRegister* reg, uint16_t val) {
  relay_seconds = val;
  saveSettings();
  return val;
}
uint16_t get_RELAY_SECONDS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t set_FLOW_STOP_SECONDS_callback(TRegister* reg, uint16_t val) {
  flow_stop_seconds = val;
  saveSettings();
  return val;
}
uint16_t get_FLOW_STOP_SECONDS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_RELAY_LATCHMODE_callback(TRegister* reg, uint16_t val) {
  relay_latchmode = val;
  saveSettings();
  return val;
}
uint16_t get_RELAY_LATCHMODE_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_HORN_LATCHMODE_callback(TRegister* reg, uint16_t val) {
  horn_latchmode = val;
  saveSettings();
  return val;
}
uint16_t get_HORN_LATCHMODE_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_OUTPUT_PULSE_MS_callback(TRegister* reg, uint16_t val) {
  output_pulse_ms = val;
  saveSettings();
  return val;
}
uint16_t get_OUTPUT_PULSE_MS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_OUTPUT_PULSE_OFF_MS_callback(TRegister* reg, uint16_t val) {
  output_pulse_off_ms = val;
  saveSettings();
  return val;
}
uint16_t get_OUTPUT_PULSE_OFF_MS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_UNITS_PER_EVENT_callback(TRegister* reg, uint16_t val) {
  units_per_event = val;
  saveSettings();
  return val;
}
uint16_t get_UNITS_PER_EVENT_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_OUTPUT_PULSES_PER_TRIGGER_callback(TRegister* reg, uint16_t val) {
  output_pulses_per_trigger = val;
  saveSettings();
  return val;
}
uint16_t get_OUTPUT_PULSES_PER_TRIGGER_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_OUTPUT_PULSES_FOR_PRIME_callback(TRegister* reg, uint16_t val) {
  output_pulses_for_prime = val;
  saveSettings();
  return val;
}
uint16_t get_OUTPUT_PULSES_FOR_PRIME_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_MIN_FLOW_callback(TRegister* reg, uint16_t val) {
  min_flow = val;
  saveSettings();
  return val;
}
uint16_t get_MIN_FLOW_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_ANALOG_THRESHOLD_callback(TRegister* reg, uint16_t val) {
  analog_threshold = val;
  saveSettings();
  return val;
}
uint16_t get_ANALOG_THRESHOLD_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_ANALOG_MAP_MIN_callback(TRegister* reg, uint16_t val) {
  analog_map_min = val;
  if (analog_map_min > 5000) {
    analog_map_min = 5000;
  }
  saveSettings();
  return val;
}
uint16_t get_ANALOG_MAP_MIN_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_ANALOG_MAP_MAX_callback(TRegister* reg, uint16_t val) {
  analog_map_max = val;
  if (analog_map_max > 5000) {
    analog_map_max = 5000;
  }
  saveSettings();
  return val;
}
uint16_t get_ANALOG_MAP_MAX_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_ANALOG_RANGE_MIN_callback(TRegister* reg, uint16_t val) {
  analog_range_min = val;
  saveSettings();
  return val;
}
uint16_t get_ANALOG_RANGE_MIN_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_ANALOG_RANGE_MAX_callback(TRegister* reg, uint16_t val) {
  analog_range_max = val;
  saveSettings();
  return val;
}
uint16_t get_ANALOG_RANGE_MAX_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t set_FLOWRATE_SECONDS_callback(TRegister* reg, uint16_t val) {
  flowRate_seconds = val;
  saveSettings();
  return val;
}
uint16_t get_FLOWRATE_SECONDS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}

uint16_t set_ADD_TOTAL_UNITS_callback(TRegister* reg, uint16_t val) {
  addUnits(val * 10);
  // u_long lftime = u_long((lifetime_units_MSW * 65536) + lifetime_units_LSW);
  // lftime = lftime + (val * 10);

  // u_long mswToAdd = lftime / 65536;
  // u_long lswToAdd = (lftime - (mswToAdd * 65536)); //remainder

  // lifetime_units_LSW = uint16_t(lswToAdd);
  // lifetime_units_MSW = uint16_t(mswToAdd);

  saveData();
  return val;
}
uint16_t get_ADD_TOTAL_UNITS_callback(TRegister* reg, uint16_t val) {
  //Serial.print("value = ");
  //Serial.println(val);
  return val;
}


uint16_t set_RESET_callback(TRegister* reg, uint16_t val) {
  if (val == 0) {
    Serial.println("RESET TYPE 0 - SAVE & RESTART ONLY");
    saveData();
    ESP.restart();  //reset ESP32
  }
  if (val == 1) {
    Serial.println("RESET TYPE 1 - RESET SETTINGS ONLY TO DEFAULTS");
    stop();
    saveData();
    resetDefaults();
  }
  if (val == 2) {
    Serial.println("RESET TYPE 2 - RESET DATA ONLY");
    stop();
    resetData();
  }
  if (val == 3) {
    Serial.println("RESET TYPE 3 - RESET WIFI ONLY AND RESTART");
    saveData();
    esp_wifi_restore();  // clear wifi credentials
    ESP.restart();       //reset ESP32
  }
  if (val == 4) {
    Serial.println("RESET TYPE 4 - RESET ALL TO DEFAULTS + WIFI AND RESTART");
    resetDefaults();
    resetData();
    esp_wifi_restore();  // clear wifi credentials
    ESP.restart();       //reset ESP32
  }
  if (val == 5) {
    Serial.println("RESET TYPE 5 - SAVE DATA AND RESTART WITH PORTAL (OTA)");
    boot_mode = 1;
    saveData();
    saveBootMode();
    ESP.restart();  //reset ESP32
  }
  if (val > 5) {
    Serial.print("Invalid reset type ");
    Serial.println(val);
    Serial.print("\n");
  }
  return val;
}


uint16_t get_RESET_callback(TRegister* reg, uint16_t val) {
  Serial.println("RESET TYPE 0 - RESTART ONLY");
  Serial.println("RESET TYPE 1 - RESET SETTINGS ONLY TO DEFAULTS");
  Serial.println("RESET TYPE 2 - RESET DATA ONLY");
  Serial.println("RESET TYPE 3 - RESET WIFI ONLY AND RESTART");
  Serial.println("RESET TYPE 4 - RESET ALL TO DEFAULTS + WIFI AND RESTART");
  Serial.println("RESET TYPE 5 - SAVE DATA AND RESTART WITH BLOCKING PORTAL (OTA)");
  return val;
}
