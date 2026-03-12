#ifndef COMPLEX_H
#define COMPLEX_H


typedef struct {
    double real;
    double imag;
} complex_t;

complex_t complex_plus(const complex_t, const complex_t);

complex_t complex_neg(const complex_t);

complex_t complex_minus(const complex_t, const complex_t);

complex_t complex_mult(const complex_t, const complex_t);

complex_t complex_conj(const complex_t);

complex_t complex_div(const complex_t, const complex_t);

complex_t complex_pow(const complex_t, const complex_t);

double complex_abs(const complex_t);

complex_t complex_log(const complex_t);

complex_t complex_exp(const complex_t);

complex_t complex_sin(const complex_t);

complex_t complex_cos(const complex_t);

complex_t complex_tan(const complex_t);

complex_t complex_asin(const complex_t);

complex_t complex_acos(const complex_t);

complex_t complex_atan(const complex_t);

complex_t complex_sinh(const complex_t);

complex_t complex_cosh(const complex_t);

complex_t complex_tanh(const complex_t);

complex_t complex_asinh(const complex_t);

complex_t complex_acosh(const complex_t);

complex_t complex_atanh(const complex_t);


#endif
