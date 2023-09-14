#include "DelimiterFramedIO.h"

DelimiterFramedIO::DelimiterFramedIO(
  AbstractBufferedIO* io,
  char delimiter)
{
  this->io = io;
  this->delimiter = delimiter;
}

int DelimiterFramedIO::read(char *buffer, size_t sz)
{
  int bytes = this->io->peek(buffer, sz);
  for (int i=0; i<bytes; i++) {
    if (buffer[i] == this->delimiter) {
      return this->io->read(buffer,
        static_cast<size_t>(i) + 1);
    }
  }

  // Wenn kein vollständiges Frame gefunden wurde geben wir nichts
  // zurück
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

