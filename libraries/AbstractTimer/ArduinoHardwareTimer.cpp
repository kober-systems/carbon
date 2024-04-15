#ifndef PLATFORM_ARDUINO_DISABLED

#include "ArduinoHardwareTimer.h"

#include <Arduino.h>

ArduinoHardwareTimer::ArduinoHardwareTimer() {}

uint32_t ArduinoHardwareTimer::now() {
  //return millis();

  //_sps.cont.act_ms = millis();
  //_sps.cont.system_sec = Teensy3Clock.get();
  return Teensy3Clock.get();
}

void ArduinoHardwareTimer::current_time_set(uint32_t timestamp) {
  Teensy3Clock.set(now());
}

#endif // PLATFORM_ARDUINO_DISABLED
