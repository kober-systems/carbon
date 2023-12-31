#ifndef DelimitedIO_h_INCLUDED
#define DelimitedIO_h_INCLUDED

#include "AbstractIO.h"

class DelimiterFramedIO: public AbstractBufferedIO {
public:
    enum class parse_state {
        // The parser has not found a delimiter
        not_found,
        // The parser has found the start of a delimiter // but hasn't parsed it fully
        partially,
        // A full delimiter was found
        found
    };

    DelimiterFramedIO(AbstractBufferedIO *io);
    DelimiterFramedIO(AbstractBufferedIO *io, parse_state (*check_delimiter)(char, size_t));

    int read(char *buffer, size_t sz);
    int write(const char *buffer, size_t sz);
    int peek(char *buffer, size_t sz);

private:
    AbstractBufferedIO* io = nullptr;
    parse_state (*check_delimiter)(char, size_t) = nullptr; // Function pointer for custom delimiter checking
};

#endif
