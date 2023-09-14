#ifndef io_h_INCLUDED
#define io_h_INCLUDED

// Interface für eine allgemeine io Schnittstelle
//
// Zum großen Teil wird das einem Subset der Stream Klasse in Arduino
// entsprechen. Diese lokale Schnittstelle wird definiert um leicht auf
// neue Systeme außerhalb von Arduino anpassen zu können.
//
// Arduino Stream Referenz:
// https://www.arduino.cc/reference/en/language/functions/communication/stream/

#include <stddef.h>

class AbstractIO {
public:
  virtual int read(char *buffer, size_t sz) = 0;

  virtual int write(const char *buffer, size_t sz) = 0;
};

class AbstractBufferedIO: public AbstractIO {
public:
  // Liest die nächsten `sz` Bytes eines Streams aus. Beim nächsten
  // `read` sind diese Bytes weiterhin im Ausgabepuffer vorhanden. Ruft
  // man `peek` mehrfach auf, so werden immer die nächsten Bytes aus dem
  // Stream ausgelesen.
  virtual int peek(char *buffer, size_t sz) = 0;
};

#endif // io_h_INCLUDED

