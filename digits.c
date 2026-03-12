#include <math.h>
#include "digits.h"


size_t get_digit_count(const uint64_t value, const uint64_t radix) {
    double valueLog = log(value);
    double radixLog = log(radix);

    return (size_t)(valueLog / radixLog) + 1;
}

uint64_t get_digit(const uint64_t value, const size_t index) {
    return value / (uint64_t)pow(DEFAULT_BASE, index - 1) % DEFAULT_BASE;
}
