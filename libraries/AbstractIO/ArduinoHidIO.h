// Implementierung der allgemeinen `io` Schnittstelle für Arduino

#ifndef ArduinoHid_h_INCLUDED
#define ArduinoHid_h_INCLUDED

#ifndef PLATFORM_ARDUINO_DISABLED

#include "AbstractIO.h"

#include <usb_rawhid.h>

// TODO This should be taken from the arduino platform headers
#define HID_BUFFER_SIZE 64

class ArduinoHidIO: public AbstractBufferedIO {
public:
  // Die Schnittstelle initialisieren
  //
  // port:      Der gewrappte Arduino Stream
  // buffer:    Der statische Puffer, der als interner Puffer für `peek`
  //            verwendet wird.
  // buffer_sz: Die Größe des staischen Puffers in bytes
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

#endif // PLATFORM_ARDUINO_DISABLED
#endif // ArduinoHid_h_INCLUDED

