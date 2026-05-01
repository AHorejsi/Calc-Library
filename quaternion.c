#include <math.h>
#include "floating_points.h"
#include "quaternion.h"


quaternion_t from_real_to_quaternion(const double real) {
    quaternion_t val = { real, 0, 0, 0 };

    return val;
}

quaternion_t from_complex_to_quaternion(const complex_t* com) {
    quaternion_t val = { com->real, com->imag, 0, 0 };

    return val;
}

bool quaternion_isnan(const quaternion_t* num) {
    return isnan(num->real) || isnan(num->imag0) || isnan(num->imag1) || isnan(num->imag2);
}

quaternion_t quaternion_plus(const quaternion_t* left, const quaternion_t* right) {
    double newReal = left->real + right->real;
    double newImag0 = left->imag0 + right->imag0;
    double newImag1 = left->imag1 + right->imag1;
    double newImag2 = left->imag2 + right->imag2;

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;
}

quaternion_t quaternion_neg(const quaternion_t* num) {
    double newReal = -(num->real);
    double newImag0 = -(num->imag0);
    double newImag1 = -(num->imag1);
    double newImag2 = -(num->imag2);

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;
}

quaternion_t quaternion_minus(const quaternion_t* left, const quaternion_t* right) {
    quaternion_t negRight = quaternion_neg(right);
    quaternion_t result = quaternion_plus(left, &negRight);

    return result;
}

quaternion_t quaternion_mult(const quaternion_t* left, const quaternion_t* right) {
    double newReal = left->real * right->real - left->imag0 * right->imag0 - left->imag1 * right->imag1 - left->imag2 * right->imag2;
    double newImag0 = left->imag0 * right->real + left->real * right->imag0 - left->imag2 * right->imag1 + left->imag1 * right->imag2;
    double newImag1 = left->imag1 * right->real + left->imag2 * right->imag0 + left->real * right->imag1 - left->imag0 * right->imag2;
    double newImag2 = left->imag2 * right->real - left->imag1 * right->imag0 + left->imag0 * right->imag1 + left->real * right->imag2;

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;
}

quaternion_t quaternion_conj(const quaternion_t* num) {
    double newReal = num->real;
    double newImag0 = -(num->imag0);
    double newImag1 = -(num->imag1);
    double newImag2 = -(num->imag2);

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;
}

quaternion_t quaternion_inv(const quaternion_t* num) {
    quaternion_t numerator = quaternion_conj(num);
    double denominator = num->real * num->real + num->imag0 * num->imag0 + num->imag1 * num->imag1 + num->imag2 * num->imag2;

    double newReal = numerator.real / denominator;
    double newImag0 = numerator.imag0 / denominator;
    double newImag1 = numerator.imag1 / denominator;
    double newImag2 = numerator.imag2 / denominator;

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;
}

quaternion_t quaternion_div(const quaternion_t* left, const quaternion_t* right) {
    double newReal = left->real * right->real + left->imag0 * right->imag0 + left->imag1 * right->imag1 + left->imag2 * right->imag2;
    double newImag0 = left->imag0 * right->real - left->real * right->imag0 - left->imag2 * right->imag1 + left->imag1 * right->imag2;
    double newImag1 = left->imag1 * right->real + left->imag2 * right->imag0 - left->real * right->imag1 - left->imag0 * right->imag2;
    double newImag2 = left->imag2 * right->real - left->imag1 * right->imag0 + left->imag0 * right->imag1 - left->real * right->imag2;

    double denominator = right->real * right->real + right->imag0 * right->imag0 + right->imag1 * right->imag1 + right->imag2 * right->imag2;

    newReal /= denominator;
    newImag0 /= denominator;
    newImag1 /= denominator;
    newImag2 /= denominator;

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;

}

quaternion_t quaternion_pow(const quaternion_t* base, const quaternion_t* exponent) {
    quaternion_t baseLog = quaternion_log(base);
    quaternion_t factorExp = quaternion_mult(exponent, &baseLog);

    return quaternion_exp(&factorExp);
}

quaternion_t quaternion_sqrt(const quaternion_t* num) {
    const quaternion_t HALF = { 0.5, 0, 0, 0 };

    return quaternion_pow(num, &HALF);
}

double quaternion_abs(const quaternion_t* num) {
    return sqrt(num->real * num->real + num->imag0 * num->imag0 + num->imag1 * num->imag1 + num->imag2 * num->imag2);
}

quaternion_t quaternion_norm(const quaternion_t* num) {
    double absVal = quaternion_abs(num);

    double newReal = num->real / absVal;
    double newImag0 = num->imag0 / absVal;
    double newImag1 = num->imag1 / absVal;
    double newImag2 = num->imag2 / absVal;

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;
}

quaternion_t quaternion_vector(const quaternion_t* num) {
    quaternion_t result = { 0, num->imag0, num->imag1, num->imag2 };

    return result;
}

quaternion_t quaternion_log(const quaternion_t* num) {
    quaternion_t a = quaternion_vector(num);
    double b = quaternion_abs(&a);
    quaternion_t c = from_real_to_quaternion(b);
    quaternion_t d = quaternion_div(&a, &c);

    double e = num->real / quaternion_abs(num);
    double f = acos(e);
    quaternion_t g = from_real_to_quaternion(f);

    quaternion_t h = quaternion_mult(&d, &g);
    double i = log(b);
    quaternion_t j = from_real_to_quaternion(i);

    return quaternion_plus(&j, &h);
}

quaternion_t quaternion_log10(const quaternion_t* num) {
    const quaternion_t TEN = { 10, 0, 0, 0 };

    return quaternion_log_base(&TEN, num);
}

quaternion_t quaternion_log2(const quaternion_t* num) {
    const quaternion_t TWO = { 2, 0, 0, 0 };

    return quaternion_log_base(&TWO, num);
}

quaternion_t quaternion_log_base(const quaternion_t* base, const quaternion_t* value) {
    quaternion_t numerator = quaternion_log(value);
    quaternion_t denominator = quaternion_log(base);

    return quaternion_div(&numerator, &denominator);
}

quaternion_t quaternion_exp(const quaternion_t* num) {
    quaternion_t a = quaternion_vector(num);
    double b = quaternion_abs(&a);
    quaternion_t c = from_real_to_quaternion(b);

    double d = sin(b);
    quaternion_t e = from_real_to_quaternion(d);
    quaternion_t f = quaternion_mult(&c, &e);

    double g = cos(b);
    quaternion_t h = from_real_to_quaternion(g);
    quaternion_t i = quaternion_plus(&h, &f);

    double j = exp(num->real);
    quaternion_t k = from_real_to_quaternion(j);

    return quaternion_mult(&k, &i);
}

bool quaternion_equal(const quaternion_t* left, const quaternion_t* right) {
    return nearly_equal(left->real, right->real) && nearly_equal(left->imag0, right->imag0) && nearly_equal(left->imag1, right->imag1) && nearly_equal(left->imag2, right->imag2);
}
