#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <stddef.h>


typedef struct {
    double* values;
    double* probs;
    size_t count;
} prob_dist_t;

typedef struct {
    double prob;
    size_t count;
} bin_dist_t;

double prob_dist_mean(const prob_dist_t*);

double prob_dist_var(const prob_dist_t*);

double prob_dist_stddev(const prob_dist_t*);

double bin_dist_mean(const bin_dist_t*);

double bin_dist_var(const bin_dist_t*);

double bin_dist_stddev(const bin_dist_t*);


#endif
