#ifdef PLATFORM_ARDUINO_ENABLED

#include "ArduinoUsbIO.h"

ArduinoUsbIO::ArduinoUsbIO(Stream *port, char *buffer, size_t buffer_sz) {
  this->port = port;
  this->internal_buffer = buffer;
  this->internal_buffer_sz = buffer_sz;
  this->internal_buffer_filled_sz = 0;
}

int ArduinoUsbIO::read(char *buffer, size_t sz) {
  size_t out_sz = 0;

  // Wenn der interne Puffer gefüllt ist wird zunächst aus diesem
  // gelesen
  if (this->internal_buffer_filled_sz > 0) {
    while (out_sz <= this->internal_buffer_filled_sz) {
      if (out_sz >= sz) { break; }
      buffer[out_sz] = this->internal_buffer[out_sz];
      out_sz++;
    }

    // Die gelesenen Bytes aus dem internen Puffer entfernen
    for (size_t i=0; i<(this->internal_buffer_filled_sz - out_sz); i++) {
      this->internal_buffer[i] = this->internal_buffer[out_sz + i];
    }
    this->internal_buffer_filled_sz -= out_sz;
  }

  // Falls noch mehr Platz im Ausgabepuffer vorhanden ist die übrigen
  // Bytes direkt auslesen.
  int recv_bytes = this->port->readBytes(&buffer[out_sz], sz-out_sz);
  if (recv_bytes < 0) { return recv_bytes; }
  return out_sz + static_cast<size_t>(recv_bytes);
}

int ArduinoUsbIO::write(const char *buffer, size_t sz) {
  return this->port->write(buffer, sz);
}

int ArduinoUsbIO::peek(char *buffer, size_t sz) {
  // TODO Prüfen welche Anpassungen gemacht werden muessen, jetzt wo peek_size entfernt wurde

  // Prüfen wie viel Platz im internen Puffer vorhanden ist
  size_t out_sz = this->internal_buffer_sz - this->internal_buffer_filled_sz;
  if (out_sz > sz) {
    out_sz = sz;
  }

  // Die Bytes in den internen Puffer einlesen
  int recv_bytes = this->port->readBytes(
    &this->internal_buffer[this->internal_buffer_filled_sz],
    out_sz);
  if (recv_bytes < 0) { return recv_bytes; }

  // Die Bytes in den Lesepuffer übertragen
  for (int i=0; i<recv_bytes; i++) {
    buffer[i] = this->internal_buffer[this->internal_buffer_filled_sz + i];
  }

  this->internal_buffer_filled_sz += static_cast<size_t>(recv_bytes);
  return recv_bytes;
}

#endif // PLATFORM_ARDUINO_ENABLED
