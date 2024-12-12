// Implementation of the generic `io` interface for Arduino

#ifndef ArduinoUsb_h_INCLUDED
#define ArduinoUsb_h_INCLUDED

#ifdef PLATFORM_ARDUINO_ENABLED

#include "AbstractIO.h"

#include "Arduino.h"

class ArduinoUsbIO: public AbstractBufferedIO {
public:
  // Initialize the interface
  //
  // port:      The wrapped arduino stream
  // buffer:    The static buffer, wich will be used as
  //            internal buffer for `peek`
  // buffer_sz: The size of the static buffer in bytes
  ArduinoUsbIO(Stream *port, char *buffer = nullptr, size_t buffer_sz = 0);

  int read(char *buffer, size_t sz);
  int write(const char *buffer, size_t sz);
  int peek(char *buffer, size_t sz);

private:
  Stream* port = nullptr;
  char *internal_buffer = nullptr;
  size_t internal_buffer_sz = 0;
  size_t internal_buffer_filled_sz = 0;
};

#endif // PLATFORM_ARDUINO_ENABLED
#endif // ArduinoUsb_h_INCLUDED

