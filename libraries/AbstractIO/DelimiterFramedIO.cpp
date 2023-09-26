#include "DelimiterFramedIO.h"

DelimiterFramedIO::DelimiterFramedIO(AbstractBufferedIO* io, char delimiter)
    : io(io), delimiter(delimiter), check_delimiter(nullptr)
{}

DelimiterFramedIO::DelimiterFramedIO(AbstractBufferedIO* io, parse_state (*check_delimiterFunc)(char, size_t))
    : io(io), check_delimiter(check_delimiterFunc)
{}

int DelimiterFramedIO::read(char *buffer, size_t sz)
{
  int bytes = this->io->peek(buffer, sz);

  if (this->check_delimiter) { // If a delimiter checking function is provided
    for (size_t i = 0; i < bytes && i < sz; i++) {
      if (this->check_delimiter(buffer[i], i) == parse_state::found) {
         return this->io->read(buffer, i + 1);
      }
    }
    return 0;
  }

  // Otherwise, use the single delimiter
  for (int i=0; i<bytes; i++) {
    if (buffer[i] == this->delimiter) {
      return this->io->read(buffer,
        static_cast<size_t>(i) + 1);
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
