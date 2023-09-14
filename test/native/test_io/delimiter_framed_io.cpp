#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include "DelimiterFramedIO.h"
#include "FakeIO.h"

#define BUFFER_SZ 256

#define STR_SZ(string) sizeof(string)-1
#define STR(string) string, STR_SZ(string)

SCENARIO("Delimiter Protocol") {
  char buffer[BUFFER_SZ];
  auto usb = FakeIO();

  GIVEN("We have a newline delimited protocol") {
    auto proto = DelimiterFramedIO(&usb, '\n');

    WHEN("a string is send but without newline") {
      usb.simulate_send(STR("my test string"));

    THEN("no data is received yet") {
      int recv_bytes = proto.read(buffer, BUFFER_SZ);

      REQUIRE(recv_bytes == 0);

    WHEN("a newline is submitted") {
      usb.simulate_send(STR(".\n"));

    THEN("it receives the string till the newline") {
      recv_bytes = proto.read(buffer, BUFFER_SZ);

      CHECK(recv_bytes == STR_SZ("my test string.\n"));
      CHECK(std::string(buffer, recv_bytes) == "my test string.\n");
    }}}}
  }

    GIVEN("We have a newline delimited protocol") {
    auto proto = DelimiterFramedIO(&usb, '\n');

    WHEN("a string is send but multiple newlines") {
      usb.simulate_send(STR("my test\nxstring\n"));

    THEN("we only receive the first string") {
      int recv_bytes = proto.read(buffer, BUFFER_SZ);

      CHECK(recv_bytes == STR_SZ("my test\n"));
      CHECK(std::string(buffer, recv_bytes) == "my test\n");

    WHEN("we try to receive another line") {
      recv_bytes = proto.read(buffer, BUFFER_SZ);

    THEN("it receives the string till the next newline") {
      CHECK(recv_bytes == STR_SZ("xstring\n"));
      CHECK(std::string(buffer, recv_bytes) == "xstring\n");
    }}}}
  }

}

int main(int argc, char **argv)
{
  doctest::Context context;

  // BEGIN:: PLATFORMIO REQUIRED OPTIONS
  context.setOption("success", true);
  context.setOption("no-exitcode", true);
  // END:: PLATFORMIO REQUIRED OPTIONS

  context.applyCommandLine(argc, argv);
  return context.run();
}
