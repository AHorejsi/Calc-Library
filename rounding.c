#include <math.h>
#include <stdlib.h>
#include "digits.h"
#include "rounding.h"

#define BASE_ROUNDING 10


static uint32_t power_of_ten(const uint8_t power) {
    return (uint32_t)pow(BASE_ROUNDING, power);
}

double real_rounded(const double value, const uint8_t places) {
    uint32_t factor = power_of_ten(places);

    return round(value * factor) / factor;
}

static uint32_t sigfig_integer(const int32_t intPart, const uint8_t places) {
    uint32_t factor = power_of_ten(places);
    int32_t intDiv = intPart / factor;

    return intDiv * factor;
}

static double sigfig_decimal(const double value, const uint8_t places, const uint8_t lengthOfIntPart) {
    uint32_t targetLengthOfDecimalPart = places - lengthOfIntPart;

    return real_rounded(value, targetLengthOfDecimalPart);
}

double real_sigfig(const double value, const uint8_t places) {
    int32_t intPart = (int32_t)value;
    uint8_t lengthOfIntPart = get_digit_count(intPart, BASE_ROUNDING);

    if (lengthOfIntPart >= places) {
        return sigfig_integer(intPart, places);
    }
    else {
        return sigfig_decimal(value, places, lengthOfIntPart);
    }
}

complex_t complex_rounded(const complex_t* value, const uint8_t places) {
    double newReal = real_rounded(value->real, places);
    double newImag = real_rounded(value->imag, places);

    complex_t result = { newReal, newImag };

    return result;
}

complex_t complex_sigfig(const complex_t* value, const uint8_t places) {
    double newReal = real_sigfig(value->real, places);
    double newImag = real_sigfig(value->imag, places);

    complex_t result = { newReal, newImag };

    return result;
}

quaternion_t quaternion_rounded(const quaternion_t* value, const uint8_t places) {
    double newReal = real_rounded(value->real, places);
    double newImag0 = real_rounded(value->imag0, places);
    double newImag1 = real_rounded(value->imag1, places);
    double newImag2 = real_rounded(value->imag2, places);

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;
}

quaternion_t quaternion_sigfig(const quaternion_t* value, const uint8_t places) {
    double newReal = real_sigfig(value->real, places);
    double newImag0 = real_sigfig(value->imag0, places);
    double newImag1 = real_sigfig(value->imag1, places);
    double newImag2 = real_sigfig(value->imag2, places);

    quaternion_t result = { newReal, newImag0, newImag1, newImag2 };

    return result;
}
