#include <Arduino.h>
#include "unity_config.h"

extern "C" void setUp(void) {}
extern "C" void tearDown(void) {}

extern "C" void unityOutputStart(unsigned long baudrate) {
  Serial1.begin(baudrate);
}

extern "C" void unityOutputChar(unsigned int c) {
  Serial1.write(c);
}

extern "C" void unityOutputFlush(void) {
  Serial1.flush();
}

extern "C" void unityOutputComplete(void) {
  Serial1.end();
}
