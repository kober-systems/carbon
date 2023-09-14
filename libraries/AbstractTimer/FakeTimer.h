#ifndef FakeTimer_h_INCLUDED
#define FakeTimer_h_INCLUDED

#include "AbstractTimer.h"

class FakeTimer: public AbstractTimer {
public:
  FakeTimer();

  uint32_t now();
  void current_time_set(uint32_t timestamp);

  // Die Variablen im FakeTimer sind public. Der Grund dafür ist,
  // dass der FakeTimer ohnehin nur in Tests verwendet werden sollte.
  // In diesem Umfeld hat man gerne einen Einblick in die internen
  // Variablen der Hardware (Simulation).
  uint32_t timestamp = 0;
  uint32_t auto_increase = 1;
};

#endif // FakeTimer_h_INCLUDED

