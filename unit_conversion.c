#include <math.h>
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


typedef double(*convert_t)(const double, const double);

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
    if (initial == target) {
        return value;
    }

    int8_t exponentOfInitial = find_exponent(initial);
    int8_t exponentOfTarget = find_exponent(target);

    return value * pow(10, exponentOfTarget - exponentOfInitial);
}

static double conversion(const double value, const uint8_t initial, const uint8_t target, const convert_t op, const double* converter) {
    double result = value;

    for (uint8_t current = initial; current != target; ++current) {
        result = op(result, converter[current]);
    }

    return result;
}

static double multiply(const double left, const double right) {
    return left * right;
}

static double divide(const double left, const double right) {
    return left / right;
}

double convert_distance(const double value, const distance_unit_t initial, const distance_unit_t target) {
    if (initial == target) {
        return value;
    }
 
    static const double converter[4] = { 0.0254, 12, 3, 1760 };

    if (initial < target) {
        return conversion(value, initial, target, multiply, converter);
    }
    else {
        return conversion(value, target, initial, divide, converter);
    }
}

double convert_time(const double value, const time_unit_t initial, const time_unit_t target) {
    if (initial == target) {
        return value;
    }

    static const double converter[5] = { 60, 60, 24, 7, 52 };

    if (initial < target) {
        return conversion(value, initial, target, multiply, converter);
    }
    else {
        return conversion(value, target, initial, divide, converter);
    }
}

double convert_weight(const double value, const weight_unit_t initial, const weight_unit_t target) {
    if (initial == target) {
        return value;
    }

    static const double converter[3] = { 0.035274, 16, 2000 };

    if (initial < target) {
        return conversion(value, initial, target, multiply, converter);
    }
    else {
        return conversion(value, target, initial, divide, converter);
    }
}

double convert_volume(const double value, const volume_unit_t initial, const volume_unit_t target) {
    if (initial == target) {
        return value;
    }

    static const double converter[4] = { 0.0295735, 16, 2, 4 };

    if (initial < target) {
        return conversion(value, initial, target, multiply, converter);
    }
    else {
        return conversion(value, target, initial, divide, converter);
    }
}

static double to_celsius(const double temperature, const temperature_unit_t unit) {
    double result;

    switch (unit) {
    case FAHRENHEIT:
        result = 5 * (temperature - 32) / 9;

        break;
    case CELSIUS:
        result = temperature;

        break;
    case KELVIN:
        result = temperature - 273.15;

        break;
    }

    return result;
}

static double from_celsius(const double celsius, const temperature_unit_t unit) {
    double result;

    switch (unit) {
    case FAHRENHEIT:
        result = (9 * celsius / 5) + 32;

        break;
    case CELSIUS:
        result = celsius;

        break;
    case KELVIN:
        result = celsius + 273.15;

        break;
    }

    return result;
}

double convert_temperature(const double value, const temperature_unit_t initial, const temperature_unit_t target) {
    if (initial == target) {
        return value;
    }

    double celsius = to_celsius(value, initial);
    double result = from_celsius(celsius, target);

    return result;
}
