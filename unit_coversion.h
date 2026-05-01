#ifndef UNIT_CONVERSION_H
#define UNIT_CONVERSION_H

#include <stdint.h>


typedef enum {
    PICO,
    NANO,
    MICRO,
    MILLI,
    CENTI,
    DECI,
    NONE,
    DECA,
    HECTO,
    KILO,
    MEGA,
    GIGA,
    TERA
} unit_prefix_t;

typedef enum {
    METER,
    INCH,
    FEET,
    YARD,
    MILE
} distance_unit_t;

typedef enum {
    SECOND,
    MINUTE,
    HOUR,
    DAY,
    WEEK,
    YEAR
} time_unit_t;

typedef enum {
    GRAM,
    OUNCE,
    POUND,
    TON
} weight_unit_t;

typedef enum {
    LITER,
    FLUID_OUNCE,
    PINT,
    QUART,
    GALLON
} volume_unit_t;

typedef enum {
    KELVIN,
    CELSIUS,
    FAHRENHEIT
} temperature_unit_t;

double prefix_convert(const double, const unit_prefix_t, const unit_prefix_t);

double convert_distance(const double, const distance_unit_t, const distance_unit_t);

double convert_time(const double, const time_unit_t, const time_unit_t);

double convert_weight(const double, const weight_unit_t, const weight_unit_t);

double convert_volume(const double, const volume_unit_t, const volume_unit_t);

double convert_temperature(const double, const temperature_unit_t, const temperature_unit_t);


#endif
