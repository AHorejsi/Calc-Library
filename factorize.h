#ifndef FACTORIZE_H
#define FACTORIZE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define END_FACTOR 0


typedef struct {
    uint32_t lower;
    uint32_t higher;
} factor_pair_t;

uint32_t* multiples(const uint32_t, const size_t);

size_t factorize(const uint32_t, uint32_t*, const size_t);

size_t pair_factorize(const uint32_t, factor_pair_t*, const size_t);

bool is_prime(const uint32_t);

bool are_coprime(const uint32_t, const uint32_t);

size_t prime_factorize(const uint32_t, uint32_t*, const size_t);

uint32_t* primes_under(const uint32_t);


#endif
