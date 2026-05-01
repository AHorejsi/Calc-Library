#include <math.h>
#include "distributions.h"


double prob_dist_mean(const prob_dist_t* dist) {
    size_t count = dist->count;
    double* valuePtr = dist->values;
    double* probPtr = dist->probs;

    double result = 0;

    for (size_t index = 0; index < count; ++index) {
        result += (*valuePtr) * (*probPtr);

        ++valuePtr;
        ++probPtr;
    }

    return result / count;
}

double prob_dist_var(const prob_dist_t* dist) {
    size_t count = dist->count;
    double* valuePtr = dist->values;
    double* probPtr = dist->probs;
    double meanValue = prob_dist_mean(dist);

    double result = 0;

    for (size_t index = 0; index < count; ++index) {
        double factor = (*valuePtr) - meanValue;
        result += factor * (*probPtr);

        ++valuePtr;
        ++probPtr;
    }

    return result;
}

double prob_dist_stddev(const prob_dist_t* dist) {
    return sqrt(prob_dist_var(dist));
}

double bin_dist_mean(const bin_dist_t* dist) {
    return dist->prob * dist->count;
}

double bin_dist_var(const bin_dist_t* dist) {
    return bin_dist_mean(dist) * (1 - dist->prob);
}

double bin_dist_stddev(const bin_dist_t* dist) {
    return sqrt(bin_dist_var(dist));
}
