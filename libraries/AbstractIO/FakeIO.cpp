#include "FakeIO.h"

FakeIO::FakeIO() {}

static int
do_read(char *buffer, size_t sz, FakeIO::inner_buffer *io) {
  size_t out_sz = io->buffer.size();
  if (out_sz > sz) {
    out_sz = sz;
  }
  for (size_t i=0; i<out_sz; i++) {
    buffer[i] = io->buffer[i];
  }
  io->peek_pos -= out_sz;
  io->buffer.erase(0, out_sz);

  return out_sz;
}

static int
do_write(const char *buffer, size_t sz, FakeIO::inner_buffer *io) {
  io->buffer += std::string(buffer, sz);
  return sz;
}

static int
do_peek(char *buffer, size_t sz, FakeIO::inner_buffer *io) {
  size_t out_sz = io->buffer.size();
  if (out_sz > sz) {
    out_sz = sz;
  }

  // Die Bytes in den Lesepuffer übertragen
  for (size_t i=0; i<out_sz; i++) {
    buffer[i] = io->buffer[i];
  }

  return out_sz;
}

int FakeIO::read(char *buffer, size_t sz) {
  return do_read(buffer, sz, &this->read_buffer);
}

int FakeIO::write(const char *buffer, size_t sz) {
  return do_write(buffer, sz, &this->write_buffer);
}

int FakeIO::peek(char *buffer, size_t sz) {
  return do_peek(buffer, sz, &this->read_buffer);
}

int FakeIO::simulate_send(const char *buffer, size_t sz) {
  return do_write(buffer, sz, &this->read_buffer);
}

int FakeIO::simulate_recv(char *buffer, size_t sz) {
  return do_read(buffer, sz, &this->write_buffer);
}

int FakeIO::simulate_peek(char *buffer, size_t sz) {
  return do_peek(buffer, sz, &this->write_buffer);
}

