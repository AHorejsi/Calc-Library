#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <stddef.h>


typedef struct {
    double* values;
    size_t length;
    size_t maxCount;
} mode_t;

double sum(const double*, const size_t);

double* cumsum(const double*, const size_t);

double prod(const double*, const size_t);

double* cumprod(const double*, const size_t);

double minimum(const double*, const size_t);

double maximum(const double*, const size_t);

double mean(const double*, const size_t);

double hmean(const double*, const size_t);

double gmean(const double*, const size_t);

double pmean(const double*, const size_t, const double);

double median(const double*, const size_t);

mode_t mode(const double*, const size_t);

double range(const double*, const size_t);

double midrange(const double*, const size_t);

double variance(const double*, const size_t);

double stddev(const double*, const size_t);


#endif
