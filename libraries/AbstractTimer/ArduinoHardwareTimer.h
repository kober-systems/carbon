#ifndef ArduinoHardwareTimer_h_INCLUDED
#define ArduinoHardwareTimer_h_INCLUDED

#ifndef PLATFORM_ARDUINO_DISABLED

#include "AbstractTimer.h"

class ArduinoHardwareTimer: public AbstractTimer {
public:
  ArduinoHardwareTimer();

  uint32_t now();
  void current_time_set(uint32_t timestamp);
};

#endif // PLATFORM_ARDUINO_DISABLED
#endif // ArduinoHardwareTimer_h_INCLUDED

