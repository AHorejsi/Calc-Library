#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdbool.h>


typedef struct {
    double real;
    double imag;
} complex_t;

extern const complex_t ZERO_COMPLEX;
extern const complex_t ONE_COMPLEX;
extern const complex_t TWO_COMPLEX;
extern const complex_t IMAG_COMPLEX;
extern const complex_t NAN_COMPLEX;

complex_t from_real(const double);

complex_t from_polar(const double, const double);

bool complex_isnan(const complex_t*);

complex_t complex_plus(const complex_t*, const complex_t*);

complex_t complex_neg(const complex_t*);

complex_t complex_minus(const complex_t*, const complex_t*);

complex_t complex_mult(const complex_t*, const complex_t*);

complex_t complex_conj(const complex_t*);

complex_t complex_inv(const complex_t*);

complex_t complex_div(const complex_t*, const complex_t*);

complex_t complex_pow(const complex_t*, const complex_t*);

complex_t complex_sqrt(const complex_t*);

double complex_abs(const complex_t*);

complex_t complex_norm(const complex_t*);

double complex_phase(const complex_t*);

complex_t complex_log(const complex_t*);

complex_t complex_log10(const complex_t*);

complex_t complex_log2(const complex_t*);

complex_t complex_log_base(const complex_t*, const complex_t*);

complex_t complex_exp(const complex_t*);

complex_t complex_sin(const complex_t*);

complex_t complex_cos(const complex_t*);

complex_t complex_tan(const complex_t*);

complex_t complex_sinh(const complex_t*);

complex_t complex_cosh(const complex_t*);

complex_t complex_tanh(const complex_t*);

complex_t complex_asin(const complex_t*);

complex_t complex_acos(const complex_t*);

complex_t complex_atan(const complex_t*);

complex_t complex_asinh(const complex_t*);

complex_t complex_acosh(const complex_t*);

complex_t complex_atanh(const complex_t*);

bool complex_equal(const complex_t*, const complex_t*);

size_t complex_rectangular_string(const complex_t*, char*);

size_t complex_polar_string(const complex_t*, char*);


#endif
