#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <stddef.h>
#include "complex.h"


typedef struct {
    double* nums;
    size_t count;
} real_list_t;

typedef struct {
    complex_t* nums;
    size_t count;
} complex_list_t;

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

extern const real_list_t EMPTY_REAL_LIST;
extern const complex_list_t EMPTY_COMPLEX_LIST;

void destroy_real_list(real_list_t*);

void destroy_complex_list(complex_list_t*);

double real_sum(const real_list_t*);

complex_t complex_sum(const complex_list_t*);

real_list_t real_cumsum(const real_list_t*);

complex_list_t complex_cumsum(const complex_list_t*);

double real_prod(const real_list_t*);

complex_t complex_prod(const complex_list_t*);

real_list_t real_cumprod(const real_list_t*);

complex_list_t complex_cumprod(const complex_list_t*);

double real_minimum(const real_list_t*);

double real_maximum(const real_list_t*);

double real_mean(const real_list_t*);

complex_t complex_mean(const complex_list_t*);

double real_hmean(const real_list_t*);

complex_t complex_hmean(const complex_list_t*);

double real_gmean(const real_list_t*);

complex_t complex_gmean(const complex_list_t*);

double real_pmean(const real_list_t*, const double);

complex_t complex_pmean(const complex_list_t*, const complex_t*);

double real_median(const real_list_t*);

real_mode_t real_mode(const real_list_t*);

complex_mode_t complex_mode(const complex_t*, const size_t); // TODO

double real_range(const real_list_t*);

double real_midrange(const real_list_t*);

double real_variance(const real_list_t*);

complex_t complex_variance(const complex_list_t*);

double real_stddev(const real_list_t*);

complex_t complex_stddev(const complex_list_t*);

double real_select(const real_list_t*, const size_t);

void real_sort(const real_list_t*);


#endif
