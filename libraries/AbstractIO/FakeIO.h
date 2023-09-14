#ifndef FakeIO_h_INCLUDED
#define FakeIO_h_INCLUDED

#include "AbstractIO.h"

#include <string>

class FakeIO: public AbstractBufferedIO {
public:
  FakeIO();

  int read(char *buffer, size_t sz);
  int write(const char *buffer, size_t sz);
  int peek(char *buffer, size_t sz);

  // Simulieren, dass der Schnittstelle von außen
  // Daten geschickt werden
  int simulate_send(const char *buffer, size_t sz);
  // Simulieren, dass aus der Schnittstelle von außen
  // Daten gelesen werden
  int simulate_recv(char *buffer, size_t sz);
  int simulate_peek(char *buffer, size_t sz);

  typedef struct {
    std::string buffer;
    size_t peek_pos = 0;
  } inner_buffer;

private:
  inner_buffer read_buffer;
  inner_buffer write_buffer;
};

#endif // FakeIO_h_INCLUDED

