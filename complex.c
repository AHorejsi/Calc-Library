#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include "floating_points.h"
#include "string_utils.h"
#include "complex.h"


const complex_t ZERO_COMPLEX = { 0, 0 };
const complex_t ONE_COMPLEX = { 1, 0 };
const complex_t TWO_COMPLEX = { 2, 0 };
const complex_t IMAG_COMPLEX = { 0, 1 };
const complex_t NAN_COMPLEX = { NAN, NAN };

complex_t from_real(const double real) {
    complex_t com = { real, 0 };

    return com;
}

complex_t from_polar(const double radius, const double angle) {
    double newReal = radius * cos(angle);
    double newImag = radius * sin(angle);

    complex_t result = { newReal, newImag };

    return result;
}

bool complex_isnan(const complex_t* num) {
    return isnan(num->real) || isnan(num->imag);
}

complex_t complex_plus(const complex_t* left, const complex_t* right) {
    double newReal = left->real + right->real;
    double newImag = left->imag + right->imag;

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_neg(const complex_t* num) {
    double newReal = -(num->real);
    double newImag = -(num->imag);

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_minus(const complex_t* left, const complex_t* right) {
    complex_t negRight = complex_neg(right);
    complex_t result = complex_plus(left, &negRight);

    return result;
}

complex_t complex_mult(const complex_t* left, const complex_t* right) {
    double newReal = left->real * right->real - left->imag * right->imag;
    double newImag = left->real * right->imag + left->imag * right->real;

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_conj(const complex_t* num) {
    double newReal = num->real;
    double newImag = -(num->imag);

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_inv(const complex_t* num) {
    return complex_div(&ONE_COMPLEX, num);
}

complex_t complex_div(const complex_t* left, const complex_t* right) {
    complex_t rightConj = complex_conj(right);

    complex_t numerator = complex_mult(left, &rightConj);
    double denominator = complex_mult(right, &rightConj).real;

    double newReal = numerator.real / denominator;
    double newImag = numerator.imag / denominator;

    complex_t result = { newReal, newImag };

    return result;
}

double complex_abs(const complex_t* num) {
    return sqrt(num->real * num->real + num->imag * num->imag);
}

complex_t complex_pow(const complex_t* base, const complex_t* exponent) {
    complex_t baseLog = complex_log(base);
    complex_t factorExp = complex_mult(exponent, &baseLog);

    return complex_exp(&factorExp);
}

complex_t complex_sqrt(const complex_t* num) {
    const complex_t HALF = { 0.5, 0 };

    return complex_pow(num, &HALF);
}

complex_t complex_norm(const complex_t* num) {
    double absVal = complex_abs(num);

    double newReal = num->real / absVal;
    double newImag = num->imag / absVal;

    complex_t result = { newReal, newImag };

    return result;
}

double complex_phase(const complex_t* num) {
    return atan2(num->imag, num->real);
}

static complex_t log_real(const double real) {
    double newReal;
    double newImag;

    if (real > 0) {
        newReal = log(real);
        newImag = 0;
    }
    else {
        newReal = log(-real);
        newImag = M_PI;
    }

    complex_t result = { newReal, newImag };

    return result;
}

static complex_t log_imag(const double imag) {
    double newReal;
    double newImag;

    if (imag > 0) {
        newReal = log(imag);
        newImag = M_PI_2;
    }
    else {
        newReal = log(-imag);
        newImag = -M_PI_2;
    }

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_log(const complex_t* num) {
    bool zeroReal = nearly_equal(0, num->real);
    bool zeroImag = nearly_equal(0, num->imag);

    if (zeroReal && zeroImag) {
        return NAN_COMPLEX;
    }
    else if (zeroReal) {
        return log_imag(num->imag);
    }
    else if (zeroImag) {
        return log_real(num->real);
    }
    else {
        double newReal = log(complex_abs(num));
        double newImag = complex_phase(num);

        complex_t result = { newReal, newImag };

        return result;
    }
}

complex_t complex_log10(const complex_t* num) {
    const complex_t TEN = { 10, 0 };

    return complex_log_base(num, &TEN);
}

complex_t complex_log2(const complex_t* num) {
    const complex_t TWO = { 2, 0 };

    return complex_log_base(num, &TWO);
}

complex_t complex_log_base(const complex_t* base, const complex_t* value) {
    complex_t numerator = complex_log(value);
    complex_t denominator = complex_log(base);

    return complex_div(&numerator, &denominator);
}

complex_t complex_exp(const complex_t* num) {
    double factor = exp(num->real);

    double newReal = factor * cos(num->imag);
    double newImag = factor * sin(num->imag);

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_sin(const complex_t* num) {
    double newReal = sin(num->real) * cosh(num->imag);
    double newImag = cos(num->real) * sinh(num->imag);

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_cos(const complex_t* num) {
    double real = num->real + M_PI / 2;
    double imag = num->imag;

    complex_t arg = { real, imag };

    return complex_sin(&arg);
}

complex_t complex_tan(const complex_t* num) {
    complex_t numerator = complex_sin(num);
    complex_t denominator = complex_cos(num);

    return complex_div(&numerator, &denominator);
}

complex_t complex_sinh(const complex_t* num) {
    complex_t arg = complex_mult(&IMAG_COMPLEX, num);
    complex_t fac = complex_sin(&arg);
    complex_t coef = complex_neg(&IMAG_COMPLEX);

    return complex_mult(&coef, &fac);
}

complex_t complex_cosh(const complex_t* num) {
    complex_t arg = complex_mult(&IMAG_COMPLEX, num);

    return complex_cos(&arg);
}

complex_t complex_tanh(const complex_t* num) {
    complex_t numerator = complex_sinh(num);
    complex_t denominator = complex_cosh(num);

    return complex_div(&numerator, &denominator);
}

complex_t complex_asin(const complex_t* num) {
    complex_t a = complex_mult(num, num);
    complex_t b = complex_minus(&ONE_COMPLEX, &a);
    complex_t c = complex_sqrt(&b);

    complex_t d = complex_mult(&IMAG_COMPLEX, num);
    complex_t e = complex_minus(&c, &d);

    complex_t f = complex_log(&e);
    complex_t g = complex_mult(&IMAG_COMPLEX, &f);

    return g;
}

complex_t complex_acos(const complex_t* num) {
    complex_t arcsin = complex_asin(num);
    complex_t ADJUST = { M_PI / 2, 0 };

    return complex_minus(&ADJUST, &arcsin);
}

complex_t complex_atan(const complex_t* num) {
    complex_t a = complex_minus(&IMAG_COMPLEX, num);
    complex_t b = complex_plus(&IMAG_COMPLEX, num);
    complex_t c = complex_div(&a, &b);
    complex_t d = complex_log(&c);

    complex_t e = complex_div(&IMAG_COMPLEX, &TWO_COMPLEX);
    complex_t f = complex_neg(&e);
    complex_t g = complex_mult(&f, &d);

    return g;
}

complex_t complex_asinh(const complex_t* num) {
    complex_t a = complex_mult(num, num);
    complex_t b = complex_plus(&a, &ONE_COMPLEX);
    complex_t c = complex_sqrt(&b);

    complex_t d = complex_plus(num, &c);
    complex_t e = complex_log(&d);

    return e;
}

complex_t complex_acosh(const complex_t* num) {
    complex_t a = complex_minus(num, &ONE_COMPLEX);
    complex_t b = complex_sqrt(&a);

    complex_t c = complex_plus(num, &ONE_COMPLEX);
    complex_t d = complex_sqrt(&c);
    
    complex_t e = complex_mult(&b, &d);
    complex_t f = complex_plus(num, &e);
    complex_t g = complex_log(&f);

    return g;
}

complex_t complex_atanh(const complex_t* num) {
    complex_t a = complex_plus(&ONE_COMPLEX, num);
    complex_t b = complex_log(&a);

    complex_t c = complex_minus(&ONE_COMPLEX, num);
    complex_t d = complex_log(&c);

    complex_t e = complex_minus(&b, &d);
    complex_t f = complex_div(&ONE_COMPLEX, &TWO_COMPLEX);
    complex_t g = complex_mult(&f, &e);

    return g;
}

bool complex_equal(const complex_t* left, const complex_t* right) {
    return nearly_equal(left->real, right->real) && nearly_equal(left->imag, right->imag);
}

static void make_zero_string(char* str, size_t* length) {
    *str = '0';
    *length = 1;
}

static void make_real_string(char* str, const double value, size_t* length) {
    *length = sprintf(str, "%lf", value);
}

size_t complex_rectangular_string(const complex_t* num, char* str) {
    bool isRealZero = nearly_equal(0, num->real);
    bool isImagZero = nearly_equal(0, num->imag);

    size_t length = 0;

    if (isRealZero && isImagZero) {
        make_zero_string(str, &length);
    }
    else if (!isRealZero && isImagZero) {
        make_real_string(str, num->real, &length);
        
        str[length++] = 'i';
    }
    else if (isRealZero && !isImagZero) {
        make_real_string(str, num->real, &length);
    }
    else {
        make_real_string(str, num->real, &length);

        str[length++] = ' ';
        str[length++] = num->imag < 0 ? '-' : '+';
        str[length++] = ' ';

        make_real_string(str + length, fabs(num->imag), &length);
    }

    str[length] = END_CHAR;

    return length;
}

size_t complex_polar_string(const complex_t* num, char* str) {
    double radius = complex_abs(num);
    double phase = complex_phase(num);

    bool isRadiusZero = nearly_equal(0, radius);
    bool isPhaseZero = nearly_equal(0, num->imag);

    size_t length = 0;

    if (isRadiusZero) {
        make_zero_string(str, &length);
    }
    else if (isPhaseZero) {
        make_real_string(str, radius, &length);
    }
    else {
        make_real_string(str, radius, &length);

        str[length++] = ' ';
        str[length++] = '*';
        str[length++] = ' ';
        str[length++] = 'e';
        str[length++] = '^';
        str[length++] = '(';

        make_real_string(str + length, phase, &length);

        str[length++] = ' ';
        str[length++] = '*';
        str[length++] = ' ';
        str[length++] = 'i';
        str[length++] = ')';
    }

    str[length] = END_CHAR;

    return length;
}
