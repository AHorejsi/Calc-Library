#ifndef RATIONAL_H
#define RATIONAL_H

#include <stdint.h>
#include <stdbool.h>


typedef enum {
    INVALID,
    NEUTRAL,
    POSITIVE,
    NEGATIVE
} sign_t;

typedef struct {
    sign_t sign;
    uint64_t numerator;
    uint64_t denominator;
} rational_t;

extern const rational_t INVALID_RATIONAL;
extern const rational_t ZERO_RATIONAL;
extern const rational_t ONE_RATIONAL;

rational_t from_decimal(const double);

rational_t from_integer(const int64_t);

double to_decimal(const rational_t*);

rational_t make_rational(const int64_t, const int64_t);

bool is_integer(const rational_t*);

rational_t rational_plus(const rational_t*, const rational_t*);

rational_t rational_negate(const rational_t*);

rational_t rational_minus(const rational_t*, const rational_t*);

rational_t rational_mult(const rational_t*, const rational_t*);

rational_t rational_div(const rational_t*, const rational_t*);

rational_t rational_pow(const rational_t*, const rational_t*);

rational_t rational_sqrt(const rational_t*);

rational_t rational_cbrt(const rational_t*);

rational_t rational_reciprocal(const rational_t*);

rational_t rational_abs(const rational_t*);

rational_t rational_min(const rational_t*, const rational_t*);

rational_t rational_max(const rational_t*, const rational_t*);

rational_t rational_ceil(const rational_t*);

rational_t rational_floor(const rational_t*);

rational_t rational_round(const rational_t*);

rational_t rational_log(const rational_t*, const rational_t*);

rational_t rational_exp(const rational_t*);

bool rational_equal(const rational_t*, const rational_t*);

bool rational_lesser(const rational_t*, const rational_t*);

bool rational_greater(const rational_t*, const rational_t*);

bool rational_lesser_or_equal(const rational_t*, const rational_t*);

bool rational_greater_or_equal(const rational_t*, const rational_t*);

char* rational_string(const rational_t*);

char* remainder_string(const rational_t*);

void real_string(const rational_t*, char*, const size_t, const size_t);


#endif
