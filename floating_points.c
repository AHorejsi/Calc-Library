#include <math.h>
#include <float.h>
#include "floating_points.h"

bool nearly_equal(const double value1, const double value2) {
    if (value1 == value2) {
        return true;
    }

    double abs1 = fabs(value1);
    double abs2 = fabs(value2);
    double diff = fabs(value1 - value2);
    double sum = abs1 + abs2;

    if (0 == value1 || 0 == value2 || sum < DBL_MIN) {
        return diff < (DBL_EPSILON * DBL_MIN);
    }
    else {
        return diff / fmin(sum, DBL_MAX) < DBL_EPSILON;
    }
}
