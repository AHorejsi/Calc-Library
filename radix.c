#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "alloc.h"
#include "string_utils.h"
#include "digits.h"
#include "radix.h"

#define RADIX_BINARY 2
#define RADIX_HEX 16
#define RADIX_OCTAL 8
#define RADIX_DECIMAL 10


#define BINARY_DIGIT_SET "01"
#define OCTAL_DIGIT_SET "01234567"
#define HEX_DIGIT_SET "0123456789ABCDEF"
#define DECIMAL_DIGIT_SET "0123456789"


const char POS_SIGN = '+';
const char NEG_SIGN = '-';

static char to_char(const uint8_t index, const char* digitSet) {
    return (index < RADIX_DECIMAL) ? index + MIN_DIGIT : digitSet[index - RADIX_DECIMAL];
}

static uint8_t from_char(const char digit, const char* digitSet) {
    if (MIN_DIGIT <= digit && digit <= MAX_DIGIT)  {
        return digit - MIN_DIGIT;
    }
    else {
        const char* startPtr = &digit;
        char* ptr = strstr(digitSet, startPtr);

        return (ptr - startPtr) + RADIX_DECIMAL;
    }
}

static uint8_t pad_for_sign(const char start) {
    return (POS_SIGN == start || NEG_SIGN == start) ? 1 : 0;
}

static uint64_t power_of_ten(const char* value, const uint8_t startIndex) {
    size_t exponent = strlen(value) - startIndex - 1;
    uint64_t result = 1;

    for (size_t count = 0; count < exponent; ++count) {
        result *= RADIX_DECIMAL;
    }

    return result;
}

static uint64_t to_integer(const char* value, const uint8_t radix, const char* digitSet) {
    uint64_t result = 0;

    int8_t startIndex = pad_for_sign(value[0]);
    uint64_t power = power_of_ten(value, startIndex);

    for (char* ptr = (char*)(value + startIndex); *ptr != END_CHAR; ++ptr) {
        uint8_t digit = from_char(*ptr, digitSet);

        result += digit * power;
        power /= RADIX_DECIMAL;
    }

    return result;
}

static void to_string(const uint64_t decimal, const char sign, const uint8_t radix, const char* digitSet, char** result, size_t* length) {
    uint8_t startIndex = pad_for_sign(sign);

    size_t digitLength = get_digit_count(decimal, radix);
    size_t totalLength = digitLength + startIndex + 1;

    char* str = (char*)malloc(totalLength * sizeof(char));
    check_alloc(str);

    uint64_t remain = decimal;

    for (ptrdiff_t index = totalLength - 2; index >= startIndex; --index) {
        uint8_t digitIndex = remain % radix;
        remain /= radix;

        str[index] = to_char(digitIndex, digitSet);
    }

    str[totalLength - 1] = END_CHAR;

    if (POS_SIGN == sign) {
        str[0] = POS_SIGN;
    }
    else if (NEG_SIGN == sign) {
        str[0] = NEG_SIGN;
    }

    *length = totalLength - 1;
    *result = str;
}

static bool matches_radix(const char* value, const uint8_t radix, const char* digitSet) {
    if (is_null_or_empty(value)) {
        return false;
    }

    uint8_t startIndex = pad_for_sign(value[0]);

    if (END_CHAR == value[startIndex]) {
        return false;
    }

    for (char* ptr = (char*)(value + startIndex); *ptr != END_CHAR; ++ptr) {
        char digit = *ptr;

        if (digit < MIN_DIGIT || digit > MAX_DIGIT) {
            return false;
        }

        if (!string_contains(digitSet, digit)) {
            return false;
        }
    }

    return true;
}

static size_t to_base(const char* value, const uint8_t radix, char** result, const char* digitSet) {
    size_t length = 0;

    if (!matches_radix(value, RADIX_DECIMAL, DECIMAL_DIGIT_SET)) {
        *result = NULL;
    }
    else {
        uint64_t decimal = to_integer(value, RADIX_DECIMAL, DECIMAL_DIGIT_SET);
        printf("TEST0: %u\n", decimal);

        to_string(decimal, value[0], radix, digitSet, result, &length);
    }

    return length;
}

size_t to_binary(const char* decimal, char** result) {
    return to_base(decimal, RADIX_BINARY, result, BINARY_DIGIT_SET);
}

size_t to_hex(const char* decimal, char** result) {
    return to_base(decimal, RADIX_HEX, result, HEX_DIGIT_SET);
}

size_t to_octal(const char* decimal, char** result) {
    return to_base(decimal, RADIX_OCTAL, result, OCTAL_DIGIT_SET);
}

static size_t from_base(const char* value, uint8_t radix, char** result, const char* digitSet) {
    size_t length = 0;

    if (!matches_radix(value, radix, digitSet)) {
        *result = NULL;
    }
    else {
        uint64_t decimal = to_integer(value, radix, digitSet);
        to_string(decimal, value[0], RADIX_DECIMAL, DECIMAL_DIGIT_SET, result, &length);
    }

    return length;
}

size_t from_binary(const char* binary, char** result) {
    return from_base(binary, RADIX_BINARY, result, BINARY_DIGIT_SET);
}

size_t from_hex(const char* hex, char** result) {
    return from_base(hex, RADIX_HEX, result, HEX_DIGIT_SET);
}

size_t from_octal(const char* octal, char** result) {
    return from_base(octal, RADIX_OCTAL, result, OCTAL_DIGIT_SET);
}
