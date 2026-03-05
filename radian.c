#define _USE_MATH_DEFINES

#include <math.h>
#include "radian.h"


double to_radians(const double degrees) {
    return degrees * (M_PI / 180);
}

double unit_circle(const double index, const double divisor) {
    return (2 * M_PI * index) / divisor;
}
