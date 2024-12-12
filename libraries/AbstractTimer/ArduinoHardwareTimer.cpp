#ifdef PLATFORM_ARDUINO_ENABLED

#include "ArduinoHardwareTimer.h"

#include <Arduino.h>

ArduinoHardwareTimer::ArduinoHardwareTimer() {}

#ifdef PLATFORM_TEENSY_ENABLED
uint32_t ArduinoHardwareTimer::now() {
  return Teensy3Clock.get();
}

void ArduinoHardwareTimer::current_time_set(uint32_t timestamp) {
  Teensy3Clock.set(now());
}
#else
uint32_t ArduinoHardwareTimer::now() {
  return millis();
}

void ArduinoHardwareTimer::current_time_set(uint32_t timestamp) {
  // TODO
}
#endif // PLATFORM_TEENSY_ENABLED

#endif // PLATFORM_ARDUINO_ENABLED
