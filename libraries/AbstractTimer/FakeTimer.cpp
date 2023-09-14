#include "FakeTimer.h"

FakeTimer::FakeTimer() {}

uint32_t FakeTimer::now() {
  uint32_t rc = this->timestamp;
  this->timestamp += this->auto_increase;

  return rc;
}

void FakeTimer::current_time_set(uint32_t timestamp) {
  this->timestamp = timestamp;
}

