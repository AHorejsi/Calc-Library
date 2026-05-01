#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <stddef.h>
#include "complex.h"


typedef struct {
    double* values;
    size_t length;
    size_t maxCount;
} real_mode_t;

typedef struct {
    complex_t* values;
    size_t length;
    size_t maxCount;
} complex_mode_t;

double real_sum(const double*, const size_t);

complex_t complex_sum(const complex_t*, const size_t);

double* real_cumsum(const double*, const size_t);

complex_t* complex_cumsum(const complex_t*, const size_t);

double real_prod(const double*, const size_t);

complex_t complex_prod(const complex_t*, const size_t);

double* real_cumprod(const double*, const size_t);

complex_t* complex_cumprod(const complex_t*, const size_t);

double real_minimum(const double*, const size_t);

double real_maximum(const double*, const size_t);

double real_mean(const double*, const size_t);

complex_t complex_mean(const complex_t*, const size_t);

double real_hmean(const double*, const size_t);

complex_t complex_hmean(const complex_t*, const size_t);

double real_gmean(const double*, const size_t);

complex_t complex_gmean(const complex_t*, const size_t);

double real_pmean(const double*, const size_t, const double);

complex_t complex_pmean(const complex_t*, const size_t, const complex_t*);

double real_median(const double*, const size_t);

real_mode_t real_mode(const double*, const size_t);

complex_mode_t complex_mode(const complex_t*, const size_t);

double real_range(const double*, const size_t);

double real_midrange(const double*, const size_t);

double real_variance(const double*, const size_t);

complex_t complex_variance(const complex_t*, const size_t);

double real_stddev(const double*, const size_t);

complex_t complex_stddev(const complex_t*, const size_t);


#endif
