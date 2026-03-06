#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "alloc.h"
#include "string_utils.h"
#include "radix.h"

#define RADIX_BINARY 2
#define RADIX_HEX 16
#define RADIX_OCTAL 8
#define RADIX_DECIMAL 10

#define MIN_DIGIT '0'
#define NEG_SIGN '-'

#define HEX_DIGIT_SET "ABCDEF"
#define EMPTY_DIGIT_SET ""


static char to_char(const uint8_t index, const char* digitSet) {
    return (index < RADIX_DECIMAL) ? index + MIN_DIGIT : digitSet[index - RADIX_DECIMAL];
}

static uint8_t from_char(const char digit, const char* digitSet) {
    uint8_t index = digit - MIN_DIGIT;

    return (0 <= index && index <= 9) ? index : digitSet[index - digit];
}

static int8_t pad_for_negative_sign(const bool isNegative) {
    return isNegative ? 1 : 0;
}

static uint64_t to_integer(const char* value, const uint8_t radix, const char* digitSet, const bool isNegative) {
    uint64_t result = 0;

    int8_t startIndex = pad_for_negative_sign(isNegative);
    size_t exponent = strlen(value) - startIndex - 1;

    for (char* ptr = (char*)(value + startIndex); *ptr != END_CHAR; ++ptr) {
        uint8_t digit = from_char(*ptr, digitSet);
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
    int8_t startIndex = pad_for_negative_sign(isNegative);

    size_t endIndex = get_digit_length(decimal, radix);
    uint64_t remain = decimal;

    char* str = (char*)malloc((endIndex + startIndex + 1) * sizeof(char));
    check_alloc(str);

    for (ptrdiff_t index = endIndex - 1; index >= startIndex; --index) {
        uint8_t digitIndex = remain % radix;
        char digit = to_char(digitIndex, digitSet);

        str[index] = digit;

        remain /= radix;
    }

    str[endIndex] = END_CHAR;

    if (isNegative) {
        str[0] = NEG_SIGN;
    }

    return str;
}

static bool matches_radix(const char* value, const uint8_t radix, const char* digitSet, const bool isNegative) {
    int8_t startIndex = pad_for_negative_sign(isNegative);

    for (char* ptr = (char*)(value + startIndex); *ptr != END_CHAR; ++ptr) {
        uint8_t digit = from_char(*ptr, digitSet);

        if (digit < 0 || digit >= radix) {
            return false;
        }
    }

    return true;
}

static char* to_base(const char* value, const uint8_t radix, const char* digitSet) {
    bool isNegative = NEG_SIGN == value[0];

    if (!matches_radix(value, RADIX_DECIMAL, EMPTY_DIGIT_SET, isNegative)) {
        return NULL;
    }

    uint64_t decimal = to_integer(value, RADIX_DECIMAL, EMPTY_DIGIT_SET, isNegative);
    char* result = to_string(decimal, radix, digitSet, isNegative);

    return result;
}

char* to_binary(const char* decimal) {
    return to_base(decimal, RADIX_BINARY, EMPTY_DIGIT_SET);
}

char* to_hex(const char* decimal) {
    return to_base(decimal, RADIX_HEX, HEX_DIGIT_SET);
}

char* to_octal(const char* decimal) {
    return to_base(decimal, RADIX_OCTAL, EMPTY_DIGIT_SET);
}

static char* from_base(const char* value, uint8_t radix) {
    bool isNegative = NEG_SIGN == value[0];

    if (!matches_radix(value, radix, HEX_DIGIT_SET, isNegative)) {
        return NULL;
    }

    uint64_t decimal = to_integer(value, radix,  HEX_DIGIT_SET, isNegative);
    char* result = to_string(decimal, RADIX_DECIMAL, HEX_DIGIT_SET, isNegative);

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
