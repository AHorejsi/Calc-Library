#include <math.h>
#include "digits.h"


size_t get_digit_count(const uint64_t value, const uint64_t radix) {
    if (0 == value) {
        return 1;
    }

    double logResult = log(value) / log(radix);
    size_t logFloor = (size_t)logResult;

    return logFloor + 1;
}

uint64_t get_digit(const uint64_t value, const size_t index) {
    return value / (uint64_t)pow(DEFAULT_BASE, index - 1) % DEFAULT_BASE;
}
