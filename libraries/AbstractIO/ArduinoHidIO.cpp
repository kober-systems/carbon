#ifdef PLATFORM_ARDUINO_ENABLED
#ifdef HIDIO_ENABLED

#include "ArduinoHidIO.h"

#include "Arduino.h"

ArduinoHidIO::ArduinoHidIO(uint32_t timeout) {
  this->total_bytes = 0;
  this->read_position = 0;
  this->timeout = timeout;
}

int ArduinoHidIO::read(char *buffer, size_t sz) {
  size_t bytes_to_copy = this->peek(buffer, sz);
  this->read_position += bytes_to_copy;

  // check if the internal buffer is just filled with
  // empty data. If so discard the buffer.
  size_t pos = this->read_position;
  while(pos < this->total_bytes) {
    if (this->internal_buffer[pos] != 0) {
      // There is still some usefull data in the buffer
      return bytes_to_copy;
    }
    pos++;
  }
  this->read_position = this->total_bytes;

  return bytes_to_copy;
}

int ArduinoHidIO::write(const char *buffer, size_t sz) {
  if (sz > sizeof(internal_buffer)) {
    return -1; // Buffer too large
  }

  char padded_buffer[64]; // Create a buffer equal to the internal buffer size
  memset(padded_buffer, 0, sizeof(padded_buffer)); // Initialize with zeros

  // Copy the provided data into the padded buffer
  memcpy(padded_buffer, buffer, sz);

  // Send the padded buffer
  return RawHID.send(padded_buffer, this->timeout);
}

int ArduinoHidIO::peek(char *buffer, size_t sz) {
  size_t pos = this->read_position; // Temporarily store the current read position
  if (pos < this->total_bytes) {
    // Peek from the internal buffer
    size_t bytes_to_copy = min(sz, this->total_bytes - pos);
    memcpy(buffer, this->internal_buffer + pos, bytes_to_copy);
    return bytes_to_copy;
  } else {
    // Peek from HID if needed
    // TODO Peeking from HID changes the internal buffer and is
    //      potentially dangerous in the context of peeking. Maybe
    //      we should remove this part. However, we will need than
    //      something similar in the read function.
    if (RawHID.available() > 0) {
      this->total_bytes = RawHID.recv(internal_buffer, this->timeout);
      this->read_position = 0;

      size_t bytes_to_copy = min(sz, this->total_bytes);
      memcpy(buffer, this->internal_buffer, bytes_to_copy);
      return bytes_to_copy;
    }
  }
  return 0; // No data available
}

#endif // HIDIO_ENABLED
#endif // PLATFORM_ARDUINO_ENABLED
