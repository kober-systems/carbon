// Manual-only AFL++ target. Every byte in data is delivered to the real
// DelimiterFramedIO implementation; chunk sizes and read windows vary by case.
#include "DelimiterFramedIO.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>

namespace {

void require(bool condition) {
    if (!condition) std::abort();
}

class InputIO final : public AbstractBufferedIO {
public:
    void append(const uint8_t* bytes, size_t count) {
        pending.insert(pending.end(), bytes, bytes + count);
    }

    int peek(char* buffer, size_t size) override {
        const size_t count = std::min(size, pending.size());
        if (count) std::memcpy(buffer, pending.data(), count);
        return static_cast<int>(count);
    }

    int read(char* buffer, size_t size) override {
        const int count = peek(buffer, size);
        pending.erase(pending.begin(), pending.begin() + count);
        return count;
    }

    int write(const char*, size_t size) override {
        return static_cast<int>(size);
    }

    std::vector<uint8_t> pending;
};

// Compare against a byte-oriented reference model, including NULs and
// delimiters beyond the requested read window.
void check_window(DelimiterFramedIO& framed, InputIO& input, size_t window) {
    std::vector<char> buffer(window + 1, '\x5a');
    const std::vector<uint8_t> before = input.pending;
    const size_t visible = std::min(window, before.size());
    const int peeked = framed.peek(buffer.data(), window);
    require(peeked == static_cast<int>(visible));
    require(buffer[window] == '\x5a');
    if (visible) require(std::memcmp(buffer.data(), before.data(), visible) == 0);

    const auto end = before.begin() + visible;
    const auto delimiter = std::find(before.begin(), end, '\n');
    const size_t expected = delimiter == end ? 0 : delimiter - before.begin() + 1;
    std::fill(buffer.begin(), buffer.end(), '\x5a');
    const int received = framed.read(buffer.data(), window);
    require(received == static_cast<int>(expected));
    require(buffer[window] == '\x5a');
    if (expected) require(std::memcmp(buffer.data(), before.data(), expected) == 0);
    // No complete frame means read must not consume any of the buffered bytes.
    require(input.pending.size() == before.size() - expected);
    require(std::equal(input.pending.begin(), input.pending.end(), before.begin() + expected));
    std::fill(buffer.begin(), buffer.end(), '\x5a');
    require(framed.peek(buffer.data(), window) == static_cast<int>(std::min(window, input.pending.size())));
    require(buffer[window] == '\x5a');
    if (window && !input.pending.empty())
        require(std::memcmp(buffer.data(), input.pending.data(), std::min(window, input.pending.size())) == 0);
}

} // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Keep allocations and reference-model work bounded, even for manual replay.
    if (size > 4096) return 0;
    InputIO input;
    DelimiterFramedIO framed(&input);
    check_window(framed, input, 0);
    for (size_t offset = 0; offset < size;) {
        const size_t chunk = std::min(size - offset, size_t(1 + data[offset] % 32));
        input.append(data + offset, chunk);
        offset += chunk;
        // Try a short, sometimes zero-length read before a full-window read.
        check_window(framed, input, data[offset - 1] % (input.pending.size() + 1));
        while (!input.pending.empty()) {
            const size_t before = input.pending.size();
            check_window(framed, input, before);
            if (input.pending.size() == before) break; // incomplete frame
        }
    }
    check_window(framed, input, input.pending.size());
    return 0;
}

int main() {
    uint8_t data[4097];
    const size_t size = std::fread(data, 1, sizeof(data), stdin);
    if (std::ferror(stdin)) return 1;
    return LLVMFuzzerTestOneInput(data, size);
}
