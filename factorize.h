#ifndef FACTORIZE_H
#define FACTORIZE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define END_FACTOR 0


uint32_t* multiples(const uint32_t, const size_t);

size_t factorize(const uint32_t, uint32_t*, const size_t);

bool is_prime(const uint32_t);

size_t prime_factorize(const uint32_t, uint32_t*, const size_t);

uint32_t* primes_under(const uint32_t);


#endif
