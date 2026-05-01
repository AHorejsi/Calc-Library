#include "counting.h"


static uint64_t multiply_upward(const uint64_t value, const uint64_t target, uint64_t* factor) {
    uint64_t result = value;
    uint64_t mult = *factor;

    while (mult <= target) {
        result *= (mult++);
    }

    *factor = mult;

    return result;
}

uint64_t factorial(const uint64_t value) {
    uint64_t factor = 1;

    return multiply_upward(1, value, &factor);
}

uint64_t choose(const uint64_t total, const uint64_t amount) {
    if (amount > total) {
        return 0;
    }

    uint64_t diff = total - amount;
    uint64_t factor = 1;

    uint64_t numerator = 0;
    uint64_t denominator1 = 0;
    uint64_t denominator2 = 0;

    if (amount < diff) {
        denominator1 = multiply_upward(1, amount, &factor);
        denominator2 = multiply_upward(denominator1, diff, &factor);
        numerator = multiply_upward(denominator2, total, &factor);
    }
    else {
        denominator2 = multiply_upward(1, diff, &factor);
        denominator1 = multiply_upward(denominator2, amount, &factor);
        numerator = multiply_upward(denominator1, total, &factor);
    }

    return numerator / (denominator1 * denominator2);
}

uint64_t permute(const uint64_t total, const uint64_t amount) {
    if (amount > total) {
        return 0;
    }

    uint64_t factor = 1;

    uint64_t denominator = multiply_upward(1, total - amount, &factor);
    uint64_t numerator = multiply_upward(denominator, total, &factor);

    return numerator / denominator;
}

uint64_t choose_with_replacement(const uint64_t total, const uint64_t amount) {
    return choose(total + amount - 1, amount);
}
