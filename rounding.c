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

static uint32_t sigfig_integer(const int32_t integerPart, const uint8_t places) {
    uint32_t factor = power_of_ten(places);
    int32_t intDiv = integerPart / factor;

    return intDiv * factor;
}

static double sigfig_decimal(const double value, const uint8_t places, const uint8_t lengthOfIntegerPart) {
    uint32_t decimalPlaceLength = places - lengthOfIntegerPart;

    return rounded(value, decimalPlaceLength);
}

double sigfig(const double value, const uint8_t places) {
    int32_t integerPart = (int32_t)value;
    uint8_t lengthOfIntegerPart = (uint8_t)log10(abs(integerPart)) + 1;

    if (lengthOfIntegerPart >= places) {
        return sigfig_integer(integerPart, places);
    }
    else {
        return sigfig_decimal(value, places, lengthOfIntegerPart);
    }
}
