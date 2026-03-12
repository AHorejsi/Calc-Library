#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"
#include "digits.h"
#include "rational.h"

#define RECURSION_DEPTH_LIMIT 16
#define STRING_CONVERSION_LENGTH 20


rational_t rational_pi(void) {
    return from_decimal(M_PI);
}

rational_t rational_e(void) {
    return from_decimal(M_E);
}

static sign_t get_sign_for_int64(const int64_t value) {
    if (value < 0) {
        return NEGATIVE;
    }
    else if (value > 0) {
        return POSITIVE;
    }
    else {
        return NEUTRAL;
    }
}

rational_t from_integer(const int64_t numerator) {
    sign_t sign = get_sign_for_int64(numerator);
    uint64_t actualNumerator = (uint64_t)numerator;

    rational_t result = { sign, actualNumerator, 1 };

    return result;
}

double to_decimal(const rational_t* num) {
    if (INVALID == num->sign) {
        return NAN;
    }

    double result = num->numerator / (double)num->denominator;
    
    if (NEGATIVE == num->sign) {
        result = -result;
    }
    
    return result;
}

static size_t find_count_of_decimal_places(const double value) {
    double valueAbs = fabs(value);
    double decimalPart = valueAbs - floor(valueAbs);

    char str[STRING_CONVERSION_LENGTH];
    size_t length = snprintf(str, STRING_CONVERSION_LENGTH, "%lf", decimalPart);

    return length - 2;
}

static sign_t get_sign_for_double(const double value) {
    if (value < 0) {
        return NEGATIVE;
    }
    else if (value > 0) {
        return POSITIVE;
    }
    else {
        return NEUTRAL;
    }
}

static rational_t get_zero_rational(void) {
    static const rational_t ZERO = { NEUTRAL, 0, 1 };

    return ZERO;
}

static uint64_t gcd(uint64_t value1, uint64_t value2) {
    while (0 != value2) {
        uint64_t temp = value2;
        value2 = value1 % value2;
        value1 = temp;
    }

    return value1;
}

static rational_t simplify_rational(const sign_t sign, uint64_t numerator, uint64_t denominator) {
    uint64_t divisor = gcd(numerator, denominator);

    numerator /= divisor;
    denominator /= divisor;

    rational_t result = { sign, numerator, denominator };

    return result;
}

static rational_t get_invalid_rational(void) {
    static const rational_t INVALID_NUM = { INVALID, 0, 0 };

    return INVALID_NUM;
}

rational_t from_decimal(const double value) {
    if (isnan(value)) {
        return get_invalid_rational();
    }

    size_t decimalPlaceCount = find_count_of_decimal_places(value);

    uint64_t denominator = (uint64_t)pow(10, decimalPlaceCount);
    uint64_t numerator = (uint64_t)(value * denominator);
    sign_t sign = get_sign_for_double(value);

    return simplify_rational(sign, numerator, denominator);
}

rational_t make_rational(const int64_t numerator, const int64_t denominator) {
    if (0 == denominator) {
        return get_invalid_rational();
    }
    else if (0 == numerator) {
        return get_zero_rational();
    }
    else {
        uint64_t actualNumerator = (uint64_t)numerator;
        uint64_t actualDenominator = (uint64_t)denominator;
        uint64_t divisor = gcd(actualNumerator, actualDenominator);

        sign_t sign = (numerator < 0 ^ denominator < 0) ? NEGATIVE : POSITIVE;
        uint64_t newNumerator = actualNumerator / divisor;
        uint64_t newDenominator = actualDenominator / divisor;

        rational_t result = { sign, newNumerator, newDenominator };

        return result;
    }
}

static uint64_t adjust_by_divisor(const uint64_t commonDenominator, const rational_t* num) {
    return commonDenominator / num->denominator * num->numerator;
}

static uint64_t lcm(const uint64_t value1, const uint64_t value2) {
    return value1 / gcd(value1, value2) * value2;
}

static rational_t actual_plus_with_positives(const rational_t* left, const rational_t* right) {
    uint64_t newDenominator = lcm(left->denominator, right->denominator);
            
    uint64_t leftNumerator = adjust_by_divisor(newDenominator, left);
    uint64_t rightNumerator = adjust_by_divisor(newDenominator, right);
    uint64_t newNumerator = leftNumerator + rightNumerator;

    return simplify_rational(POSITIVE, newNumerator, newDenominator);
}

static rational_t plus_helper(const rational_t* left, const rational_t* right) {
    rational_t result;

    if (NEGATIVE == left->sign) {
        if (POSITIVE == right->sign) {
            rational_t positiveLeft = { POSITIVE, left->numerator, left->denominator };

            result = rational_minus(right, &positiveLeft);
        }
        else {
            rational_t positiveLeft = { POSITIVE, left->numerator, left->denominator };
            rational_t positiveRight = { POSITIVE, right->numerator, right->denominator };

            result = rational_plus(&positiveLeft, &positiveRight);
            result.sign = NEGATIVE;
        }
    }
    else {
        if (POSITIVE == right->sign) {
            result = actual_plus_with_positives(left, right);
        }
        else {
            rational_t positiveRight = { POSITIVE, right->numerator, right->denominator };

            result = rational_minus(left, &positiveRight);
        }
    }

    return result;
}

rational_t rational_plus(const rational_t* left, const rational_t* right) {
    if (INVALID == left->sign || INVALID == right->sign) {
        return get_invalid_rational();
    }

    if (NEUTRAL == left->sign) {
        return *right;
    }
    else if (NEUTRAL == right->sign) {
        return *left;
    }
    else {
        return plus_helper(left, right);
    }
}

rational_t rational_negate(const rational_t* num) {
    rational_t result = *num;

    switch (num->sign) {
    case POSITIVE:
        result.sign = NEGATIVE;

        break;
    case NEGATIVE:
        result.sign = POSITIVE;

        break;
    }

    return result;
}

static rational_t actual_minus_with_positives(const rational_t* left, const rational_t* right) {
    uint64_t newDenominator = lcm(left->denominator, right->denominator);

    uint64_t leftNumerator = adjust_by_divisor(newDenominator, left);
    uint64_t rightNumerator = adjust_by_divisor(newDenominator, right);

    if (leftNumerator == rightNumerator) {
        return get_zero_rational();
    }

    uint64_t newNumerator;
    sign_t newSign;

    if (leftNumerator > rightNumerator) {
        newNumerator = leftNumerator - rightNumerator;
        newSign = POSITIVE;
    }
    else {
        newNumerator = rightNumerator - leftNumerator;
        newSign = NEGATIVE;
    }

    return simplify_rational(newSign, newNumerator, newDenominator);
}

static rational_t minus_helper(const rational_t* left, const rational_t* right) {
    rational_t result;

    if (NEGATIVE == left->sign) {
        if (POSITIVE == right->sign) {
            rational_t positiveLeft = { POSITIVE, left->numerator, left->denominator };
            
            result = rational_minus(right, &positiveLeft);
        }
        else {
            rational_t positiveRight = { POSITIVE, right->numerator, right->denominator };

            result = rational_plus(&positiveRight, left);
        }
    }
    else {
        if (POSITIVE == right->sign) {
            result = actual_minus_with_positives(left, right);
        }
        else {
            rational_t positiveRight = { POSITIVE, right->numerator, right->denominator };

            result = rational_plus(left, &positiveRight);
        }
    }

    return result;
}

rational_t rational_minus(const rational_t* left, const rational_t* right) {
    if (INVALID == left->sign || INVALID == right->sign) {
        return get_invalid_rational();
    }

    if (NEUTRAL == left->sign) {
        return rational_negate(right);
    }
    else if (NEUTRAL == right->sign) {
        return *left;
    }
    else {
        return minus_helper(left, right);
    }
}

rational_t rational_mult(const rational_t* left, const rational_t* right) {
    if (INVALID == left->sign || INVALID == right->sign) {
        return get_invalid_rational();
    }

    if (NEUTRAL == left->sign || NEUTRAL == right->sign) {
        return get_zero_rational();
    }
    else {
        uint64_t newNumerator = left->numerator * right->numerator;
        uint64_t newDenominator = left->denominator * right->denominator;
        sign_t newSign = (POSITIVE == left->sign ^ POSITIVE == right->sign) ? NEGATIVE : POSITIVE;

        rational_t result = { newSign, newNumerator, newDenominator };

        return result;
    }
}

rational_t rational_div(const rational_t* left, const rational_t* right) {
    if (INVALID == left->sign || INVALID == right->sign || NEUTRAL == right->sign) {
        return get_invalid_rational();
    }

    rational_t recip = rational_reciprocal(right);

    return rational_mult(left, &recip);
}

static rational_t get_one_rational(void) {
    static rational_t ONE = { POSITIVE, 1, 1 };

    return ONE;
}

static rational_t loop_power(const rational_t* value, const uint64_t exponent) {
    rational_t result = get_one_rational();

    for (uint64_t count = 0; count < exponent; ++count) {
        result = rational_mult(&result, value);
    }

    return result;
}

static rational_t rational_power(const rational_t* value, const uint64_t exponent) {
    if (exponent <= RECURSION_DEPTH_LIMIT) {
        return loop_power(value, exponent);
    }
    else {
        uint64_t halfExp = exponent / 2;
    
        rational_t result = rational_power(value, halfExp);
        result = rational_mult(&result, &result);

        if (1 == exponent % 2) {
            result = rational_mult(value, &result);
        }

        return result;
    }
}

static rational_t rational_root(const rational_t* value, const uint64_t root) {
    rational_t result = *value;
    rational_t divisor = get_one_rational();
    rational_t rootRational = { POSITIVE, root, 1 };

    while (rational_lesser(&result, &divisor)) {
        result = rational_plus(&result, &divisor);
        result = rational_div(&result, &rootRational);
        
        divisor = rational_div(value, &result);
    }

    return result;
}

rational_t rational_pow(const rational_t* left, const rational_t* right) {
    if (INVALID == left->sign || INVALID == right->sign) {
        return get_invalid_rational();
    }

    if (NEUTRAL == right->sign) {
        return get_one_rational();
    }

    rational_t result = rational_power(left, right->numerator);
    result = rational_root(&result, right->denominator);

    if (NEGATIVE == right->sign) {
        rational_t ONE = get_one_rational();

        return rational_div(&ONE, &result);
    }

    return result;
}

rational_t rational_sqrt(const rational_t* num) {
    rational_t half = { POSITIVE, 1, 2 };

    return rational_pow(num, &half);
}

rational_t rational_cbrt(const rational_t* num) {
    rational_t third = { POSITIVE, 1, 3 };

    return rational_pow(num, &third);
}

rational_t rational_reciprocal(const rational_t* num) {
    if (INVALID == num->sign || NEUTRAL == num->sign) {
        return get_invalid_rational();
    }

    uint64_t newNumerator = num->denominator;
    uint64_t newDenominator = num->numerator;

    rational_t result = { num->sign, newNumerator, newDenominator };

    return result;
}

rational_t rational_abs(const rational_t* num) {
    rational_t result = *num;

    if (NEGATIVE == num->sign) {
        result.sign = POSITIVE;
    }

    return result;
}

rational_t rational_min(const rational_t* left, const rational_t* right) {
    return rational_lesser(left, right) ? *left : *right;
}

rational_t rational_max(const rational_t* left, const rational_t* right) {
    return rational_lesser(left, right) ? *right : *left;
}

bool rational_equal(const rational_t* left, const rational_t* right) {
    return left->sign == right->sign && left->numerator == right->numerator && left->denominator == right->denominator;
}

bool rational_lesser(const rational_t* left, const rational_t* right) {
    if (INVALID == left->sign || INVALID == right->sign) {
        return false;
    }

    uint64_t divisor = lcm(left->denominator, right->denominator);

    uint64_t leftNumerator = adjust_by_divisor(divisor, left);
    uint64_t rightNumerator = adjust_by_divisor(divisor, right);

    return leftNumerator < rightNumerator;
}

bool rational_greater(const rational_t* left, const rational_t* right) {
    return rational_lesser(right, left);
}

bool rational_lesser_or_equal(const rational_t* left, const rational_t* right) {
    return rational_equal(left, right) || rational_lesser(left, right);
}

bool rational_greater_or_equal(const rational_t* left, const rational_t* right) {
    return rational_equal(left, right) || rational_greater(left, right);
}
