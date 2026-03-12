#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "error.h"
#include "unit_coversion.h"


#define EXP_PICO -12
#define EXP_NANO -9
#define EXP_MICRO -6
#define EXP_MILLI -3
#define EXP_CENTI -2
#define EXP_DECI -1
#define EXP_NONE 0
#define EXP_DECA 1
#define EXP_HECTO 2
#define EXP_KILO 3
#define EXP_MEGA 6
#define EXP_GIGA 9
#define EXP_TERA 12


static int8_t find_exponent(const unit_prefix_t prefix) {
    int8_t power;

    switch (prefix) {
    case PICO:
        power = EXP_PICO;

        break;
    case NANO:
        power = EXP_NANO;

        break;
    case MICRO:
        power = EXP_MICRO;

        break;
    case MILLI:
        power = EXP_MILLI;

        break;
    case CENTI:
        power = EXP_CENTI;

        break;
    case DECI:
        power = EXP_DECI;

        break;
    case NONE:
        power = EXP_NONE;
        
        break;
    case DECA:
        power = EXP_DECA;

        break;
    case HECTO:
        power = EXP_HECTO;

        break;
    case KILO:
        power = EXP_KILO;

        break;
    case MEGA:
        power = EXP_MEGA;

        break;
    case GIGA:
        power = EXP_GIGA;

        break;
    case TERA:
        power = EXP_TERA;

        break;
    }

    return power;
}

double prefix_convert(const double value, const unit_prefix_t initial, const unit_prefix_t target) {
    int8_t exponentOfInitial = find_exponent(initial);
    int8_t exponentOfTarget = find_exponent(target);

    return value * pow(10, exponentOfTarget - exponentOfInitial);
}

static double time_up(const double value, const time_unit_t initial, const time_unit_t target, const uint8_t* converter) {
    double result = value;

    for (time_unit_t current = initial; current < target; ++current) {
        result *= converter[current];
    }

    return result;
}

static double time_down(const double value, const time_unit_t initial, const time_unit_t target, const uint8_t* converter) {
    double result = value;

    for (time_unit_t current = initial; current > target; --current) {
        result /= converter[current];
    }

    return result;
}

double convert_time(const double value, const time_unit_t initial, const time_unit_t target) {
    static const uint8_t converter[5] = { 60, 60, 24, 7, 52 };

    if (initial < target) {
        return time_up(value, initial, target, converter);
    }
    else if (initial > target) {
        return time_down(value, initial, target, converter);
    }
    else {
        return value;
    }
}
