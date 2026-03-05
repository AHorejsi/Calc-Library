#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "alloc.h"
#include "radix.h"

#define RADIX_BINARY 2
#define RADIX_HEX 16
#define RADIX_OCTAL 8
#define RADIX_DECIMAL 10

#define END_CHAR '\0'
#define MIN_DIGIT '0'

#define NEG_SIGN '-'


static char to_char(const uint64_t value) {
    return value + MIN_DIGIT;
}

static uint64_t from_char(const char value) {
    return value - MIN_DIGIT;
}

static uint8_t pad_for_negative_sign(bool isNegative) {
    return isNegative ? 1 : 0;
}

static uint64_t to_integer(const char* value, const uint8_t radix, bool isNegative) {
    uint64_t result = 0;

    uint8_t startIndex = pad_for_negative_sign(isNegative);
    size_t exponent = strlen(value) - startIndex - 1;

    for (char* ptr = (char*)(value + startIndex); *ptr != END_CHAR; ++ptr) {
        uint8_t digit = from_char(*ptr);
        uint64_t factor = (uint64_t)pow(radix, exponent);

        result += digit * factor;

        --exponent;
    }

    return result;
}

static size_t get_digit_length(const uint64_t value, const uint8_t radix) {
    double valueLog = log(value);
    double radixLog = log(radix);

    return (size_t)(valueLog / radixLog) + 1;
}

static char* to_string(const uint64_t decimal, const uint8_t radix, const char* digitSet, bool isNegative) {
    uint8_t startIndex = pad_for_negative_sign(isNegative);

    size_t endIndex = get_digit_length(decimal, radix);
    uint64_t remain = decimal;

    char* str = (char*)malloc((endIndex + startIndex + 1) * sizeof(char));
    check_alloc(str);

    for (ptrdiff_t index = endIndex - 1; index >= startIndex; --index) {
        uint64_t digitIndex = remain % radix;
        char digit = (digitIndex < RADIX_DECIMAL) ? to_char(digitIndex) : digitSet[digitIndex - RADIX_DECIMAL];

        str[index] = digit;

        remain /= radix;
    }

    str[endIndex] = END_CHAR;

    return str;
}

static bool matches_radix(const char* value, const uint8_t radix, bool isNegative) {
    uint8_t startIndex = pad_for_negative_sign(isNegative);

    for (char* ptr = (char*)(value + startIndex); *ptr != END_CHAR; ++ptr) {
        uint64_t digit = from_char(*ptr);

        if (digit < 0 || digit >= radix) {
            return false;
        }
    }

    return true;
}

static char* to_base(const char* value, const uint8_t radix, const char* digitSet) {
    bool isNegative = NEG_SIGN == value[0];

    if (!matches_radix(value, RADIX_DECIMAL, isNegative)) {
        return NULL;
    }

    uint64_t decimal = to_integer(value, RADIX_DECIMAL, isNegative);
    char* result = to_string(decimal, radix, digitSet, isNegative);

    return result;
}

char* to_binary(const char* decimal) {
    return to_base(decimal, RADIX_BINARY, "");
}

char* to_hex(const char* decimal) {
    return to_base(decimal, RADIX_HEX, "ABCDEF");
}

char* to_octal(const char* decimal) {
    return to_base(decimal, RADIX_OCTAL, "");
}

static char* from_base(const char* value, uint8_t radix) {
    bool isNegative = NEG_SIGN == value[0];

    if (!matches_radix(value, radix, isNegative)) {
        return NULL;
    }

    uint64_t decimal = to_integer(value, radix, isNegative);
    char* result = to_string(decimal, RADIX_DECIMAL, "", isNegative);

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
