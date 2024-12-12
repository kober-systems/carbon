#ifndef io_h_INCLUDED
#define io_h_INCLUDED

// Interface for a general io interface
//
// For the most part, this will correspond to a subset of the Stream class in Arduino.
// This local interface is defined in order to easily adapt to
// new systems outside of Arduino.
//
// Arduino Stream Reference:
// https://www.arduino.cc/reference/en/language/functions/communication/stream/

#include <stddef.h>
#include <cstring>
#include <cstdio>

class AbstractIO {
public:
  virtual int read(char *buffer, size_t sz) = 0;

  virtual int write(const char *buffer, size_t sz) = 0;

  AbstractIO& operator<<(const char* str) {
    size_t sz = strlen(str);
    write(str, sz);
    return *this;
  }

  AbstractIO& operator<<(const int num) {
    constexpr size_t MAX_BUFFER_SIZE = 20;
    char buffer[MAX_BUFFER_SIZE];
    size_t sz = snprintf(buffer, MAX_BUFFER_SIZE, "%d", num);
    write(buffer, sz);
    return *this;
  }

  AbstractIO& operator<<(const bool v) {
    if (v) {
      write("true", strlen("true"));
    } else {
      write("false", strlen("false"));
    }
    return *this;
  }
};

class AbstractBufferedIO: public AbstractIO {
public:
  // Reads the next `sz` bytes of a stream. On the next
  // `read`, these bytes are still present in the output buffer. If
  // you call `peek` multiple times, it will always read the next bytes from the
  // stream (and not from the cache).
  virtual int peek(char *buffer, size_t sz) = 0;
};

#endif // io_h_INCLUDED

