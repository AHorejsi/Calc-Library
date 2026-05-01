#ifndef ROUNDING_H
#define ROUNDING_H

#include <stdint.h>
#include "complex.h"
#include "quaternion.h"
#include "vector.h"
#include "matrix.h"


double real_rounded(const double, const uint8_t);

double real_sigfig(const double, const uint8_t);

complex_t complex_rounded(const complex_t*, const uint8_t);

complex_t complex_sigfig(const complex_t*, const uint8_t);

quaternion_t quaternion_rounded(const quaternion_t*, const uint8_t);

quaternion_t quaternion_sigfig(const quaternion_t*, const uint8_t);


#endif
