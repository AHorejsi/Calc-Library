#include <math.h>
#include "rounding.h"

static uint32_t power_of_ten(const uint8_t power) {
    return (uint32_t)pow(10, power);
}

double rounded(const double value, const uint8_t places) {
    uint32_t factor = power_of_ten(places);
    double result = round(value * factor) / factor;

    return result;
}

static uint32_t sigfig_integer(const int32_t intPart, const uint8_t places) {
    uint32_t factor = power_of_ten(places);
    int32_t intDiv = intPart / factor;

    return intDiv * factor;
}

static double sigfig_decimal(const double value, const uint8_t places, const uint8_t lengthOfIntPart) {
    uint32_t targetLengthOfDecPart = places - lengthOfIntPart;

    return rounded(value, targetLengthOfDecPart);
}

double sigfig(const double value, const uint8_t places) {
    int32_t intPart = (int32_t)value;
    uint8_t lengthOfIntPart = (uint8_t)log10(abs(intPart)) + 1;

    if (lengthOfIntPart >= places) {
        return sigfig_integer(intPart, places);
    }
    else {
        return sigfig_decimal(value, places, lengthOfIntPart);
    }
}
