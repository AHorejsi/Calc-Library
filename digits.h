#ifndef DIGITS_H
#define DIGITS_H

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_BASE 10


size_t get_digit_count(const uint64_t, const uint64_t);

uint64_t get_digit(const uint64_t, const size_t);


#endif
