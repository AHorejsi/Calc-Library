#define _USE_MATH_DEFINES

#include <math.h>
#include "complex.h"


complex_t complex_plus(const complex_t left, const complex_t right) {
    double newReal = left.real + right.real;
    double newImag = left.imag + right.imag;

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_neg(const complex_t num) {
    double newReal = -(num.real);
    double newImag = -(num.imag);

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_minus(const complex_t left, const complex_t right) {
    complex_t negativeRight = complex_neg(right);
    complex_t result = complex_plus(left, negativeRight);

    return result;
}

complex_t complex_mult(const complex_t left, const complex_t right) {
    double newReal = left.real * left.real - right.real * right.real;
    double newImag = left.real * right.imag + left.imag * right.real;

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_conj(const complex_t num) {
    double newReal = num.real;
    double newImag = -(num.imag);

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_div(const complex_t left, const complex_t right) {
    complex_t conjugateRight = complex_conj(right);

    complex_t numerator = complex_mult(left, conjugateRight);
    double denominator = complex_mult(right, conjugateRight).real;

    double newReal = numerator.real / denominator;
    double newImag = numerator.imag / denominator;

    complex_t result = { newReal, newImag };

    return result;
}

double complex_abs(const complex_t num) {
    return sqrt(num.real * num.real + num.imag * num.imag);
}
