#ifndef DelimitedIO_h_INCLUDED
#define DelimitedIO_h_INCLUDED

// Transport Layer für ein durch Delimiter geframtes Protokoll

#include "AbstractIO.h"

class DelimiterFramedIO: public AbstractBufferedIO {
public:
  DelimiterFramedIO(AbstractBufferedIO *io, char delimiter);

  int read(char *buffer, size_t sz);
  int write(const char *buffer, size_t sz);
  int peek(char *buffer, size_t sz);

private:
  AbstractBufferedIO* io = nullptr;
  char delimiter = '\n';
};

#endif // DelimitedIO_h_INCLUDED

