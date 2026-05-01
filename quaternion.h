#ifndef QUATERNION_H
#define QUATERNION_H

#include <stdbool.h>
#include "complex.h"


typedef struct {
    double real;
    double imag0;
    double imag1;
    double imag2;
} quaternion_t;

quaternion_t from_real_to_quaternion(const double);

quaternion_t from_complex_to_quaternion(const complex_t*);

bool quaternion_isnan(const quaternion_t*);

quaternion_t quaternion_plus(const quaternion_t*, const quaternion_t*);

quaternion_t quaternion_neg(const quaternion_t*);

quaternion_t quaternion_minus(const quaternion_t*, const quaternion_t*);

quaternion_t quaternion_mult(const quaternion_t*, const quaternion_t*);

quaternion_t quaternion_conj(const quaternion_t*);

quaternion_t quaternion_inv(const quaternion_t*);

quaternion_t quaternion_div(const quaternion_t*, const quaternion_t*);

quaternion_t quaternion_pow(const quaternion_t*, const quaternion_t*);

quaternion_t quaternion_sqrt(const quaternion_t*);

double quaternion_abs(const quaternion_t*);

quaternion_t quaternion_norm(const quaternion_t*);

quaternion_t quaternion_vector(const quaternion_t*);

quaternion_t quaternion_log(const quaternion_t*);

quaternion_t quaternion_log10(const quaternion_t*);

quaternion_t quaternion_log2(const quaternion_t*);

quaternion_t quaternion_log_base(const quaternion_t*, const quaternion_t*);

quaternion_t quaternion_exp(const quaternion_t*);

bool quaternion_equal(const quaternion_t*, const quaternion_t*);


#endif
