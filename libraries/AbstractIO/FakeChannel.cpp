#include "FakeChannel.h"

FakeChannel::FakeChannel(FakeIO *other_side) {
  this->other_side = other_side;
}

int FakeChannel::read(char *buffer, size_t sz) {
  return this->other_side->simulate_recv(buffer, sz);
}

int FakeChannel::write(const char *buffer, size_t sz) {
  return this->other_side->simulate_send(buffer, sz);
}

int FakeChannel::peek(char *buffer, size_t sz) {
  return this->other_side->simulate_peek(buffer, sz);
}

