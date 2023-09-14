#ifndef FakeChannel_h_INCLUDED
#define FakeChannel_h_INCLUDED

// Simulationslayer für abstrakte Kommunikationskanäle
//
// Mit dieser Klasse wird es möglich Server, die auf Basisi von
// `AbstractIO` geschrieben wurden vollständig zu testen. Dazu stellt
// man einfach einen Kanal her und übergibt beiden Servern jeweils eine
// Seite des Kanals. Die Server verhalten sich dann genauso, als würden
// sie über eine physische Schnittstelle miteinander kommunizieren.
//
// ````
// auto side1 = FakeIO();
// auto side2 = FakeChannel(&side1);
// auto server = ModbusMeasureServer(&side1, ...);
// auto client = ModbusMeasureClient(&side2);
//
// client.trigger();
// server.update();
// ...
// ````
//
// Als Bild kann man sich diese Verbindung folgendermaßen vorstellen:
//
// +--------+  +-------+           +-------+  +--------+
// | Client |->| side2 |<--------->| side1 |<-| Server |
// +--------+  +-------+           +-------+  +--------+
//

#include "AbstractIO.h"

#include "FakeIO.h"

class FakeChannel: public AbstractBufferedIO {
public:
  FakeChannel(FakeIO *other_side);

  int read(char *buffer, size_t sz);
  int write(const char *buffer, size_t sz);
  int peek(char *buffer, size_t sz);

private:
  FakeIO *other_side;
};

#endif // FakeChannel_h_INCLUDED

