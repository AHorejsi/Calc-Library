#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"
#include "digits.h"
#include "string_utils.h"
#include "rational.h"

#define MIN_EXPONENT 16
#define MIN_SEARCH_SIZE 16
#define STRING_CONVERSION_LENGTH 20


const rational_t INVALID_RATIONAL = { INVALID, 0, 0 };
const rational_t ZERO_RATIONAL = { NEUTRAL, 0, 1 };
const rational_t ONE_RATIONAL = { POSITIVE, 1, 1 };

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

    double result = num->numerator / (double)(num->denominator);
    
    if (NEGATIVE == num->sign) {
        result = -result;
    }
    
    return result;
}

static size_t find_count_of_decimal_places(const double value) {
    double valueAbs = fabs(value);
    double decimalPart = valueAbs - (int64_t)(valueAbs);

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

static uint64_t gcd(uint64_t value1, uint64_t value2) {
    while (0 != value2) {
        uint64_t temp = value2;
        value2 = value1 % value2;
        value1 = temp;
    }

    return value1;
}

static rational_t simplify_rational(const sign_t sign, const uint64_t numerator, const uint64_t denominator) {
    uint64_t divisor = gcd(numerator, denominator);

    if (0 == divisor) {
        return ZERO_RATIONAL;
    }

    uint64_t newNumerator = numerator / divisor;
    uint64_t newDenominator = denominator / divisor;

    rational_t result = { sign, newNumerator, newDenominator };

    return result;
}

rational_t from_decimal(const double value) {
    if (isnan(value)) {
        return INVALID_RATIONAL;
    }

    size_t decimalPlaceCount = find_count_of_decimal_places(value);

    uint64_t denominator = (uint64_t)pow(10, decimalPlaceCount);
    uint64_t numerator = (uint64_t)(value * denominator);
    sign_t sign = get_sign_for_double(value);

    return simplify_rational(sign, numerator, denominator);
}

rational_t make_rational(const int64_t numerator, const int64_t denominator) {
    if (0 == denominator) {
        return INVALID_RATIONAL;
    }
    else if (0 == numerator) {
        return ZERO_RATIONAL;
    }
    else {
        uint64_t actualNumerator = abs(numerator);
        uint64_t actualDenominator = abs(denominator);
        uint64_t divisor = gcd(actualNumerator, actualDenominator);

        sign_t sign = (numerator < 0 ^ denominator < 0) ? NEGATIVE : POSITIVE;
        uint64_t newNumerator = actualNumerator / divisor;
        uint64_t newDenominator = actualDenominator / divisor;

        rational_t result = { sign, newNumerator, newDenominator };

        return result;
    }
}

bool is_integer(const rational_t* num) {
    return 1 == num->denominator;
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
        return INVALID_RATIONAL;
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
        return ZERO_RATIONAL;
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
        return INVALID_RATIONAL;
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
        return INVALID_RATIONAL;
    }

    if (NEUTRAL == left->sign || NEUTRAL == right->sign) {
        return ZERO_RATIONAL;
    }
    else {
        uint64_t newNumerator = left->numerator * right->numerator;
        uint64_t newDenominator = left->denominator * right->denominator;
        sign_t newSign = (POSITIVE == left->sign ^ POSITIVE == right->sign) ? NEGATIVE : POSITIVE;

        return simplify_rational(newSign, newNumerator, newDenominator);
    }
}

rational_t rational_div(const rational_t* left, const rational_t* right) {
    if (INVALID == left->sign || INVALID == right->sign || NEUTRAL == right->sign) {
        return INVALID_RATIONAL;
    }

    rational_t reciprocalRight = rational_reciprocal(right);

    return rational_mult(left, &reciprocalRight);
}

static rational_t loop_power(const rational_t* value, const uint8_t exponent) {
    rational_t result = ONE_RATIONAL;

    for (uint8_t count = 0; count < exponent; ++count) {
        result = rational_mult(&result, value);
    }

    return result;
}

static uint64_t nearest_lesser_power_of_2(uint64_t value) {
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    ++value;

    return value / 2;
}

static rational_t rational_exponent(const rational_t* value, const uint64_t exponent) {
    rational_t result = *value;
    uint64_t currentExponent = exponent;

    while (currentExponent >= MIN_EXPONENT) {
        uint64_t exponentNearestPowerOfTwo = nearest_lesser_power_of_2(currentExponent);
        uint64_t iterations = (uint64_t)log2(exponentNearestPowerOfTwo);

        for (size_t index = 0; index < iterations; ++index) {
            result = rational_mult(&result, &result);
        }

        currentExponent -= exponentNearestPowerOfTwo;
    }

    rational_t remain = loop_power(value, currentExponent);

    return rational_mult(&result, &remain);
}

static rational_t rational_root(const rational_t* value, const uint64_t root) {
    rational_t result = *value;
    rational_t divisor = ONE_RATIONAL;
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
        return INVALID_RATIONAL;
    }

    if (NEUTRAL == right->sign) {
        return ONE_RATIONAL;
    }

    rational_t result = rational_exponent(left, right->numerator);
    result = rational_root(&result, right->denominator);

    if (NEGATIVE == right->sign) {
        result = rational_reciprocal(&result);
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
        return INVALID_RATIONAL;
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

rational_t rational_ceil(const rational_t* num) {
    if (is_integer(num)) {
        return *num;
    }
    else {
        rational_t upped = rational_plus(num, &ONE_RATIONAL);

        return rational_floor(&upped);
    }
}

static rational_t middle_point(const rational_t* low, const rational_t* high, const rational_t* HALF) {
    rational_t result = rational_plus(high, low);
    result = rational_mult(&result, HALF);

    return result;
}

static rational_t floor_linear_search(const rational_t* num) {
    uint64_t denominator = num->denominator;
    uint64_t end = denominator + 1;

    for (uint64_t newNumerator = num->numerator - 1; newNumerator > end; --newNumerator) {
        if (0 == newNumerator % denominator) {
            return simplify_rational(num->sign, newNumerator, denominator);
        }
    }

    return ONE_RATIONAL;
}

static rational_t floor_binary_search(const rational_t* num) {
    const rational_t HALF = { POSITIVE, 1, 2 };

    rational_t low = rational_minus(num, &ONE_RATIONAL);
    rational_t high = *num;

    while (true) {
        rational_t middle = middle_point(&low, &high, &HALF);
        
        if (is_integer(&middle)) {
            return middle;
        }

        rational_t diff = rational_minus(num, &middle);

        if (rational_lesser(&ONE_RATIONAL, &diff)) {
            low = middle;
        }
        else {
            high = middle;
        }
    }

    return low;
}

rational_t rational_floor(const rational_t* num) {
    if (num->numerator < num->denominator) {
        return ZERO_RATIONAL;
    }
    else if (is_integer(num)) {
        return *num;
    }
    else if (abs(num->numerator - num->denominator) < MIN_SEARCH_SIZE) {
        return floor_linear_search(num);
    }
    else {
        return floor_binary_search(num);
    }
}

rational_t rational_round(const rational_t* num) {
    const rational_t HALF = { POSITIVE, 1, 2 };
    rational_t upped = rational_plus(num, &HALF);

    return rational_floor(&upped);
}

rational_t rational_exp(const rational_t* num) {
    const rational_t E = from_decimal(M_E);

    return rational_pow(&E, num);
}

static rational_t compute_k(void) {
    static rational_t k = ONE_RATIONAL;

    if (!rational_equal(&ONE_RATIONAL, &k)) {
        return k;
    }

    const uint8_t ITERATIONS_COUNT = 16;
    const rational_t TWO = { POSITIVE, 2, 1 };
    const rational_t NEG_TWO = { NEGATIVE, 2, 1 };
    
    rational_t index = ONE_RATIONAL;

    while (index.numerator < ITERATIONS_COUNT) {
        rational_t a = rational_mult(&NEG_TWO, &index);
        a = rational_pow(&TWO, &a);
        a = rational_plus(&ONE_RATIONAL, &a);
        a = rational_sqrt(&a);

        k = rational_div(&k, &a);

        ++(index.numerator);
    }

    return k;
}

rational_t rational_sin(const rational_t* num) {
    rational_t k = compute_k();

    
}

rational_t rational_cos(const rational_t* num) {
    rational_t halfPi = from_decimal(M_PI_2);
    rational_t shifted = rational_plus(num, &halfPi);

    return rational_sin(&shifted);
}

rational_t rational_tan(const rational_t* num) {
    rational_t sinValue = rational_sin(num);
    rational_t cosValue = rational_cos(num);

    return rational_div(&sinValue, &cosValue);
}

bool rational_equal(const rational_t* left, const rational_t* right) {
    return left->sign == right->sign && left->numerator == right->numerator && left->denominator == right->denominator;
}

bool rational_lesser(const rational_t* left, const rational_t* right) {
    if (left->sign == right->sign && (INVALID == left->sign || NEUTRAL == left->sign)) {
        return true;
    }
    else if (INVALID == left->sign ^ INVALID == right->sign) {
        return false;
    }
    else {
        uint64_t divisor = lcm(left->denominator, right->denominator);

        uint64_t leftNumerator = adjust_by_divisor(divisor, left);
        uint64_t rightNumerator = adjust_by_divisor(divisor, right);

        return leftNumerator < rightNumerator;
    }
}

bool rational_greater(const rational_t* left, const rational_t* right) {
    return rational_lesser(right, left);
}

bool rational_lesser_or_equal(const rational_t* left, const rational_t* right) {
    return !rational_greater(left, right);
}

bool rational_greater_or_equal(const rational_t* left, const rational_t* right) {
    return !rational_lesser(left, right);
}

static void build_string_part(char** destPtr, const char* srcPtr) {
    char* dest = *destPtr;

    for (char* ptr = (char*)srcPtr; END_CHAR != *ptr; ++ptr) {
        *dest = *ptr;
        ++dest;
    }
}

static char* build_rational_string(const char* sign, const char* numerator, const char* denominator, const size_t totalLength) {
    char* result = (char*)malloc(totalLength * sizeof(char));
    check_alloc(result);

    char* ptr = result;

    build_string_part(&ptr, sign);
    build_string_part(&ptr, "(");
    build_string_part(&ptr, numerator);
    build_string_part(&ptr, " / ");
    build_string_part(&ptr, denominator);
    build_string_part(&ptr, ")");

    *ptr = END_CHAR;

    return result;
}

char* rational_string(const rational_t* num) {
    if (INVALID == num->sign) {
        return NULL;
    }

    char numeratorStr[STRING_CONVERSION_LENGTH];
    size_t numeratorLength = sprintf(numeratorStr, "%lu", num->numerator);

    char denominatorStr[STRING_CONVERSION_LENGTH];
    size_t denominatorLength = sprintf(denominatorStr, "%lu", num->denominator);

    char* signStr = (NEGATIVE == num->sign) ? "-" : "";
    size_t signLength = strlen(signStr);
    
    size_t totalLength = signLength + numeratorLength + denominatorLength + 6;

    return build_rational_string(signStr, numeratorStr, denominatorStr, totalLength);
}
