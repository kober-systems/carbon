#ifndef DelimitedIO_h_INCLUDED
#define DelimitedIO_h_INCLUDED

// Transport Layer für ein durch Delimiter geframtes Protokoll

#include "AbstractIO.h"

class DelimiterFramedIO: public AbstractBufferedIO {
public:
  enum class parse_state {
    // the parser has not found a delimiter
    not_found,
    // the parser has found the start of a delimiter but not parsed it
    // to the end of the delimiter
    partially,
    // a full delimiter was found
    found,
  };

  DelimiterFramedIO(AbstractBufferedIO *io, char delimiter);
  DelimiterFramedIO(AbstractBufferedIO *io, parse_state (*check_delimiter)(char, size_t));

  int read(char *buffer, size_t sz);
  int write(const char *buffer, size_t sz);
  int peek(char *buffer, size_t sz);

private:
  AbstractBufferedIO* io = nullptr;
  char delimiter = '\n';
};

#endif // DelimitedIO_h_INCLUDED

