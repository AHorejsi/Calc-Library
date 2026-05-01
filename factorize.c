#include <math.h>
#include <stdlib.h>
#include "alloc.h"
#include "factorize.h"

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

static size_t do_realloc(void* ptr, size_t currentCapacity, size_t byteSize) {
    size_t newCapacity = currentCapacity * CAPACITY_INCREASE_RATE;
    void** values = (void**)ptr;

    *values = realloc(*values, newCapacity * byteSize);
    check_alloc(*values);

    return newCapacity;
}

size_t factorize(const uint32_t value, uint32_t* factors, const size_t initialCapacity) {
    if (0 == value) {
        return 0;
    }

    size_t index = 0;
    size_t currentCapacity = initialCapacity;

    for (uint32_t divisor = 1; divisor <= value; ++divisor) {
        if (0 != value % divisor) {
            continue;
        }

        factors[index] = divisor;
        ++index;

        if (index == currentCapacity) {
            currentCapacity = do_realloc(&factors, currentCapacity, sizeof(uint32_t));
        }
    }

    factors[index] = END_FACTOR;

    return index;
}

size_t pair_factorize(const uint32_t value, factor_pair_t* pairs, const size_t initialCapacity) {
    if (0 == value) {
        return 0;
    }

    size_t index = 0;
    size_t currentCapacity = initialCapacity;

    uint32_t end = (uint32_t)sqrt(value);

    for (uint32_t divisor = 1; divisor <= end; ++divisor) {
        if (0 != value % divisor) {
            continue;
        }

        uint32_t correspondingFactor = value / divisor;
        factor_pair_t pair = { divisor, correspondingFactor };

        pairs[index] = pair;
        ++index;

        if (index == currentCapacity) {
            currentCapacity = do_realloc(&pairs, currentCapacity, sizeof(factor_pair_t));
        }
    }

    factor_pair_t endPair = { END_FACTOR, END_FACTOR };
    pairs[index] = endPair;

    return currentCapacity;
}

bool is_prime(const uint32_t value) {
    if (value < 2) {
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

bool are_coprime(const uint32_t value1, const uint32_t value2) {
    if (value1 < 2 || value2 < 2) {
        return false;
    }

    uint32_t min = (value1 < value2) ? value1 : value2;
    uint32_t end = (uint32_t)sqrt(min);

    for (uint32_t divisor = 2; divisor <= end; ++divisor) {
        if (0 == value1 % divisor && 0 == value2 % divisor) {
            return false;
        }
    }

    return true;
}

size_t prime_factorize(const uint32_t value, uint32_t* factors, const size_t initialCapacity) {
    if (value <= 1) {
        return 0;
    }

    size_t currentCapacity = initialCapacity;

    uint32_t current = value;
    uint32_t divisor = 2;
    size_t index = 0;

    while (current > 1) {
        if (0 != current % divisor) {
            ++divisor;
        }
        else {
            factors[index] = divisor;

            current /= divisor;
            ++index;

            if (index == currentCapacity) {
                currentCapacity = do_realloc(&factors, currentCapacity, sizeof(uint32_t));
            }
        }
    }

    factors[index] = END_FACTOR;

    return index;
}

static bool* make_initial_primality(const uint32_t length) {
    const bool IS_PRIME = true;

    bool* primality = (bool*)falloc(length, sizeof(bool), &IS_PRIME);
    primality[0] = false;

    return primality;
}

static size_t mark_multiples(const uint32_t length, const size_t index, bool* primality) {
    size_t amountRemoved = 0;
    uint32_t prime = index + 1;

    for (size_t nextIndex = index + 2; nextIndex < length; ++nextIndex) {
        uint32_t value = nextIndex + 1;

        if (0 == value % prime) {
            primality[nextIndex] = false;
            ++amountRemoved;
        }
    }

    return amountRemoved;
}

static uint32_t* retrieve_primes(bool* primality, const uint32_t length, const size_t count) {
    uint32_t* primes = (uint32_t*)malloc(count * sizeof(uint32_t));
    check_alloc(primes);

    for (size_t index = 0; index < length; ++index) {
        if (primality[index]) {
            uint32_t value = index + 1;

            primes[index] = value;
        }
    }

    free(primality);

    return primes;
}

uint32_t* primes_under(const uint32_t value) {
    if (value <= 1) {
        return NULL;
    }

    size_t count = value - 1;
    size_t length = value;

    bool* primality = make_initial_primality(length);

    for (size_t index = 1; index < length; ++index) {
        if (primality[index]) {
            count -= mark_multiples(length, index, primality);
        }
    }

    return retrieve_primes(primality, length, count);
}
