#include "DelimiterFramedIO.h"

static DelimiterFramedIO::parse_state newline_proto(char cur, size_t pos) {
  if (cur == '\n') {
    return DelimiterFramedIO::parse_state::found;
  }
  return DelimiterFramedIO::parse_state::not_found;
}

DelimiterFramedIO::DelimiterFramedIO(AbstractBufferedIO* io)
    : io(io)
{
    this->check_delimiter = &newline_proto;
}

DelimiterFramedIO::DelimiterFramedIO(AbstractBufferedIO* io, parse_state (*check_delimiter)(char, size_t))
    : io(io), check_delimiter(check_delimiter)
{}

int DelimiterFramedIO::read(char *buffer, size_t sz)
{
  int bytes = this->io->peek(buffer, sz);

  // If no delimiter checking function is provided something is wrong
  if (not this->check_delimiter) { return 0; }

  for (size_t i=0; i<bytes; i++) {
    if (this->check_delimiter(buffer[i], i) == parse_state::found) {
      return this->io->read(buffer, i + 1);
    }
  }

  // If a complete frame is not found, return nothing
  return 0;
}

int DelimiterFramedIO::write(const char *buffer, size_t sz)
{
  return this->io->write(buffer, sz);
}

int DelimiterFramedIO::peek(char *buffer, size_t sz)
{
  return this->io->peek(buffer, sz);
}
