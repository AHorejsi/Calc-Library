#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "alloc.h"
#include "radix.h"

#define RADIX_BINARY 2
#define RADIX_HEX 16
#define RADIX_OCTAL 8
#define RADIX_DECIMAL 10

#define END_CHAR '\0'
#define MIN_DIGIT '0'

static uint64_t to_integer(const char* value, const uint8_t radix) {
    uint64_t result = 0;

    for (char* ptr = (char*)ptr; *ptr != END_CHAR; ++ptr) {
        uint8_t digit = (*ptr) - MIN_DIGIT;
        ptrdiff_t exponent = ptr - value;

        result += digit * (uint64_t)pow(radix, exponent);
    }

    return result;
}

static size_t get_digit_length(const uint64_t value, const uint8_t radix) {
    double valueLog = log(value);
    double radixLog = log(radix);

    return (size_t)(valueLog / radixLog) + 1;
}

static char* to_string(const uint64_t decimal, const uint8_t radix, const char* digitSet) {
    size_t length = get_digit_length(decimal, radix);
    uint64_t remain = decimal;

    char* str = (char*)malloc((length + 1) * sizeof(char));
    check_alloc(str);

    for (ptrdiff_t letterIndex = length - 2; length >= 0; --letterIndex) {
        uint64_t index = remain % radix;
        uint64_t baseValue = index - MIN_DIGIT;
        char digit = (index < RADIX_DECIMAL) ? baseValue : digitSet[baseValue - RADIX_DECIMAL];

        str[letterIndex] = digit;

        remain /= radix;
    }

    str[length - 1] = END_CHAR;

    return str;
}

static bool matches_radix(const char* value, const uint8_t radix) {
    for (char* ptr = (char*)value; *ptr != END_CHAR; ++ptr) {
        uint64_t digit = (*ptr) - MIN_DIGIT;

        if (digit < 0 || digit >= radix) {
            return false;
        }
    }

    return true;
}

static char* to_base(const char* value, const uint8_t radix, const char* digitSet) {
    if (!matches_radix(value, RADIX_DECIMAL)) {
        return NULL;
    }

    uint64_t decimal = to_integer(value, RADIX_DECIMAL);
    char* result = to_string(decimal, radix, digitSet);

    return result;
}

char* to_binary(const char* decimal) {
    return to_base(decimal, RADIX_BINARY, NULL);
}

char* to_hex(const char* decimal) {
    return to_base(decimal, RADIX_HEX, "ABCDEF");
}

char* to_octal(const char* decimal) {
    return to_base(decimal, RADIX_OCTAL, NULL);
}

static char* from_base(const char* value, uint8_t radix) {
    if (!matches_radix(value, radix)) {
        return NULL;
    }

    uint64_t decimal = to_integer(value, radix);
    char* result = to_string(decimal, RADIX_DECIMAL, NULL);

    return result;
}

char* from_binary(const char* binary) {
    return from_base(binary, RADIX_BINARY);
}

char* from_hex(const char* hex) {
    return from_base(hex, RADIX_HEX);
}

char* from_octal(const char* octal) {
    return from_base(octal, RADIX_OCTAL);
}
