// Implementierung der allgemeinen `io` Schnittstelle für Arduino

#ifndef ArduinoUsb_h_INCLUDED
#define ArduinoUsb_h_INCLUDED

#ifndef PLATFORM_ARDUINO_DISABLED

#include "AbstractIO.h"

#include "Arduino.h"

class ArduinoUsbIO: public AbstractBufferedIO {
public:
  // Die Schnittstelle initialisieren
  //
  // port:      Der gewrappte Arduino Stream
  // buffer:    Der statische Puffer, der als interner Puffer für `peek`
  //            verwendet wird.
  // buffer_sz: Die Größe des staischen Puffers in bytes
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

#endif // PLATFORM_ARDUINO_DISABLED
#endif // ArduinoUsb_h_INCLUDED

