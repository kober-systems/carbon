// Implementierung der allgemeinen `io` Schnittstelle für Arduino

#ifndef ArduinoHid_h_INCLUDED
#define ArduinoHid_h_INCLUDED

#ifdef PLATFORM_ARDUINO_ENABLED
#ifdef HIDIO_ENABLED

#include "AbstractIO.h"

#include <usb_rawhid.h>

// TODO This should be taken from the arduino platform headers
#define HID_BUFFER_SIZE 64

class ArduinoHidIO: public AbstractBufferedIO {
public:
  // Initialize the interface
  //
  // timeout:   timeout in ms before a `read`
  //            or `peek` will time out
  ArduinoHidIO(uint32_t timeout = 10);

  int read(char *buffer, size_t sz);
  int write(const char *buffer, size_t sz);
  int peek(char *buffer, size_t sz);

private:
  char internal_buffer[HID_BUFFER_SIZE];
  size_t total_bytes = 0;
  size_t read_position = 0;
  uint32_t timeout = 10;
};

#endif // HIDIO_ENABLED
#endif // PLATFORM_ARDUINO_ENABLED
#endif // ArduinoHid_h_INCLUDED

