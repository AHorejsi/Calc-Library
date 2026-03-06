#include <math.h>
#include <stdlib.h>
#include "alloc.h"
#include "factorize.h"

#define INITIAL_CAPACITY 16
#define CAPACITY_INCREASE_RATE 3 / 2


uint32_t* multiples(const uint32_t value, const size_t amount) {
    if (0 == amount) {
        return NULL;
    }

    uint32_t* multiples = (uint32_t*)malloc(amount * sizeof(uint32_t));
    check_alloc(multiples);

    for (size_t index = 0; index < amount; ++index) {
        multiples[index] = value * index;
    }

    return multiples;
}

static void check_for_realloc(uint32_t** values, size_t* capacity, const size_t index) {
    if (index != *capacity) {
        return;
    }

    size_t newCapacity = (*capacity) * CAPACITY_INCREASE_RATE;

    *capacity = newCapacity;
    *values = realloc(*values, newCapacity);

    check_alloc(*values);
}

uint32_t* factorize(const uint32_t value) {
    if (0 == value) {
        return NULL;
    }

    size_t capacity = INITIAL_CAPACITY;

    uint32_t* factors = (uint32_t*)malloc(INITIAL_CAPACITY * sizeof(uint32_t));
    check_alloc(factors);

    size_t index = 0;

    for (uint32_t divisor = 1; divisor <= value; ++divisor) {
        if (0 != value % divisor) {
            continue;
        }

        factors[index] = divisor;
        ++index;

        check_for_realloc(&factors, &capacity, index);
    }

    factors[index] = END_FACTOR;

    return factors;
}

bool is_prime(const uint32_t value) {
    if (value <= 1) {
        return false;
    }

    uint32_t end = (uint32_t)sqrt(value);

    for (uint32_t divisor = 2; divisor <= end; ++divisor) {
        if (0 == value % divisor) {
            return false;
        }
    }

    return true;
}

uint32_t* prime_factorize(const uint32_t value) {
    if (value <= 1) {
        return NULL;
    }

    size_t capacity = INITIAL_CAPACITY;

    uint32_t* primes = (uint32_t*)malloc(INITIAL_CAPACITY * sizeof(uint32_t));
    check_alloc(primes);

    uint32_t current = value;
    uint32_t divisor = 2;
    size_t index = 0;

    while (current > 1) {
        if (0 != current % divisor) {
            ++divisor;
        }
        else {
            primes[index] = divisor;

            current /= divisor;
            ++index;

            check_for_realloc(&primes, &capacity, index);
        }
    }

    primes[index] = END_FACTOR;

    return primes;
}
