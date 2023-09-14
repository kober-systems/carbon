#ifndef AbstractTimer_h_INCLUDED
#define AbstractTimer_h_INCLUDED

#include <stdint.h>

class AbstractTimer {
public:
  virtual uint32_t now() = 0;
  virtual void current_time_set(uint32_t timestamp) = 0;
};

#endif // AbstractTimer_h_INCLUDED

