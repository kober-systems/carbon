#include <Arduino.h>
#include <unity.h>

#include <ArduinoHidIO.h>

#include <cstring>

namespace {
char incoming[HID_BUFFER_SIZE];
int available_packets;
int receive_calls;
int send_calls;
uint32_t receive_timeout;
uint32_t send_timeout;
char sent[HID_BUFFER_SIZE];
int send_result;

void reset_hid() {
    memset(incoming, 0, sizeof(incoming));
    available_packets = 0;
    receive_calls = 0;
    send_calls = 0;
    receive_timeout = 0;
    send_timeout = 0;
    memset(sent, 0, sizeof(sent));
    send_result = 1;
}
}

extern "C" int __wrap_usb_rawhid_available(void) {
    return available_packets;
}

extern "C" int __wrap_usb_rawhid_recv(void *buffer, uint32_t timeout) {
    ++receive_calls;
    receive_timeout = timeout;
    memcpy(buffer, incoming, sizeof(incoming));
    --available_packets;
    return sizeof(incoming);
}

extern "C" int __wrap_usb_rawhid_send(const void *buffer, uint32_t timeout) {
    ++send_calls;
    send_timeout = timeout;
    memcpy(sent, buffer, sizeof(sent));
    return send_result;
}

void test_empty_input_does_not_receive() {
    reset_hid();
    ArduinoHidIO io;
    char result[4] = {};

    TEST_ASSERT_EQUAL_INT(0, io.peek(result, sizeof(result)));
    TEST_ASSERT_EQUAL_INT(0, io.read(result, sizeof(result)));
    TEST_ASSERT_EQUAL_INT(0, receive_calls);
}

void test_peek_then_read_preserves_packet_and_skips_zero_padding() {
    reset_hid();
    memcpy(incoming, "abc", 3);
    available_packets = 1;
    ArduinoHidIO io(23);
    char result[4] = {};

    TEST_ASSERT_EQUAL_INT(2, io.peek(result, 2));
    TEST_ASSERT_EQUAL_MEMORY("ab", result, 2);
    TEST_ASSERT_EQUAL_INT(2, io.peek(result, 2));
    TEST_ASSERT_EQUAL_MEMORY("ab", result, 2);
    TEST_ASSERT_EQUAL_INT(1, receive_calls);
    TEST_ASSERT_EQUAL_UINT32(23, receive_timeout);
    TEST_ASSERT_EQUAL_INT(2, io.read(result, 2));
    TEST_ASSERT_EQUAL_MEMORY("ab", result, 2);
    TEST_ASSERT_EQUAL_INT(1, io.read(result, 1));
    TEST_ASSERT_EQUAL_MEMORY("c", result, 1);
    TEST_ASSERT_EQUAL_INT(0, io.read(result, 1));
    TEST_ASSERT_EQUAL_INT(1, receive_calls);
}

void test_write_pads_binary_packet_and_rejects_oversize() {
    reset_hid();
    ArduinoHidIO io(37);
    const char data[] = {'a', '\0', 'b'};

    TEST_ASSERT_EQUAL_INT(1, io.write(data, sizeof(data)));
    TEST_ASSERT_EQUAL_INT(1, send_calls);
    TEST_ASSERT_EQUAL_UINT32(37, send_timeout);
    TEST_ASSERT_EQUAL_MEMORY(data, sent, sizeof(data));
    for (size_t i = sizeof(data); i < sizeof(sent); ++i) {
        TEST_ASSERT_EQUAL_INT(0, sent[i]);
    }
    TEST_ASSERT_EQUAL_INT(-1, io.write(data, HID_BUFFER_SIZE + 1));
    TEST_ASSERT_EQUAL_INT(1, send_calls);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_empty_input_does_not_receive);
    RUN_TEST(test_peek_then_read_preserves_packet_and_skips_zero_padding);
    RUN_TEST(test_write_pads_binary_packet_and_rejects_oversize);
    UNITY_END();
}

void loop() {}
