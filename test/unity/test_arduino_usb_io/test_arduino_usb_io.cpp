#include <Arduino.h>
#include <unity.h>

#include <ArduinoUsbIO.h>

#include <cstring>

// Stream::readBytes uses read(); a zero timeout makes an empty input deterministic.
class MemoryStream : public Stream {
public:
    MemoryStream(const char* input, size_t length) : input(input), length(length) {
        setTimeout(0);
    }

    int available() override { return static_cast<int>(length - position); }
    int read() override { return position < length ? static_cast<uint8_t>(input[position++]) : -1; }
    int peek() override { return position < length ? static_cast<uint8_t>(input[position]) : -1; }
    size_t write(uint8_t byte) override {
        if (written < sizeof(output)) output[written++] = static_cast<char>(byte);
        return 1;
    }

    const char* input;
    size_t length;
    size_t position = 0;
    char output[16] = {};
    size_t written = 0;
};

void test_read_without_peek() {
    MemoryStream stream("abc", 3);
    ArduinoUsbIO io(&stream);
    char result[4] = {};

    TEST_ASSERT_EQUAL_INT(3, io.read(result, 3));
    TEST_ASSERT_EQUAL_MEMORY("abc", result, 3);
    TEST_ASSERT_EQUAL_INT(0, io.read(result, 1));
}

void test_write_forwards_binary_data_and_length() {
    MemoryStream stream("", 0);
    ArduinoUsbIO io(&stream);
    const char bytes[] = {'a', '\0', 'b'};

    TEST_ASSERT_EQUAL_INT(3, io.write(bytes, sizeof(bytes)));
    TEST_ASSERT_EQUAL_UINT(3, stream.written);
    TEST_ASSERT_EQUAL_MEMORY(bytes, stream.output, sizeof(bytes));
}

void test_peek_preserves_bytes_for_read() {
    MemoryStream stream("abcd", 4);
    char cache[4] = {};
    ArduinoUsbIO io(&stream, cache, sizeof(cache));
    char result[4] = {};

    TEST_ASSERT_EQUAL_INT(2, io.peek(result, 2));
    TEST_ASSERT_EQUAL_MEMORY("ab", result, 2);
    TEST_ASSERT_EQUAL_INT(2, io.read(result, 2));
    TEST_ASSERT_EQUAL_MEMORY("ab", result, 2);
    TEST_ASSERT_EQUAL_INT(2, io.read(result, 2));
    TEST_ASSERT_EQUAL_MEMORY("cd", result, 2);
}

void test_multiple_peeks_return_successive_bytes_and_partial_read_keeps_remainder() {
    MemoryStream stream("abcde", 5);
    char cache[4] = {};
    ArduinoUsbIO io(&stream, cache, sizeof(cache));
    char result[4] = {};

    TEST_ASSERT_EQUAL_INT(2, io.peek(result, 2));
    TEST_ASSERT_EQUAL_MEMORY("ab", result, 2);
    TEST_ASSERT_EQUAL_INT(2, io.peek(result, 2));
    TEST_ASSERT_EQUAL_MEMORY("cd", result, 2);
    TEST_ASSERT_EQUAL_INT(1, io.read(result, 1));
    TEST_ASSERT_EQUAL_MEMORY("a", result, 1);
    TEST_ASSERT_EQUAL_INT(3, io.read(result, 3));
    TEST_ASSERT_EQUAL_MEMORY("bcd", result, 3);
    TEST_ASSERT_EQUAL_INT(1, io.read(result, 1));
    TEST_ASSERT_EQUAL_MEMORY("e", result, 1);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_read_without_peek);
    RUN_TEST(test_write_forwards_binary_data_and_length);
    RUN_TEST(test_peek_preserves_bytes_for_read);
    RUN_TEST(test_multiple_peeks_return_successive_bytes_and_partial_read_keeps_remainder);
    UNITY_END();
}

void loop() {}
