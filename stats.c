#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include "alloc.h"
#include "floating_points.h"
#include "stats.h"


double real_sum(const double* values, const size_t length) {
    double sumValue = 0;

    for (size_t index = 0; index < length; ++index) {
        sumValue += values[index];
    }

    return sumValue;
}

complex_t complex_sum(const complex_t* values, const size_t length) {
    complex_t sumValue = ZERO_COMPLEX;

    for (size_t index = 0; index < length; ++index) {
        const complex_t* current = values + index;

        sumValue = complex_plus(&sumValue, current);
    }

    return sumValue;
}

double* real_cumsum(const double* values, const size_t length) {
    if (0 == length) {
        return NULL;
    }

    double* result = (double*)malloc(length * sizeof(double));
    check_alloc(result);

    result[0] = values[0];
    for (size_t index = 1; index < length; ++index) {
        result[index] = values[index] + result[index - 1];
    }

    return result;
}

complex_t* complex_cumsum(const complex_t* values, const size_t length) {
    if (0 == length) {
        return NULL;
    }

    complex_t* result = (complex_t*)malloc(length * sizeof(complex_t));
    check_alloc(result);

    result[0] = values[0];
    for (size_t index = 1; index < length; ++index) {
        const complex_t* current = values + index;
        const complex_t* previous = result + (index - 1);

        result[index] = complex_plus(current, previous);
    }

    return result;
}

double real_prod(const double* values, const size_t length) {
    double prodValue = 1;

    for (size_t index = 0; index < length; ++index) {
        prodValue *= values[index];
    }

    return prodValue;
}

complex_t complex_prod(const complex_t* values, const size_t length) {
    complex_t prodValue = ONE_COMPLEX;

    for (size_t index = 0; index < length; ++index) {
        const complex_t* current = values + index;

        prodValue = complex_mult(&prodValue, current);
    }

    return prodValue;
}

double* real_cumprod(const double* values, const size_t length) {
    if (0 == length) {
        return NULL;
    }

    double* result = (double*)malloc(length * sizeof(double));
    check_alloc(result);

    result[0] = values[0];
    for (size_t index = 1; index < length; ++index) {
        result[index] = values[index] * result[index - 1];
    }

    return result;
}

complex_t* complex_cumprod(const complex_t* values, const size_t length) {
    if (0 == length) {
        return NULL;
    }

    complex_t* result = (complex_t*)malloc(length * sizeof(complex_t));
    check_alloc(result);

    result[0] = values[0];
    for (size_t index = 1; index < length; ++index) {
        const complex_t* current = values + index;
        const complex_t* previous = result + (index - 1);

        result[index] = complex_mult(current, previous);
    }

    return result;
}

double real_minimum(const double* values, const size_t length) {
    double result = DBL_MAX;

    for (size_t index = 0; index < length; ++index) {
        double current = values[index];

        if (current < result) {
            result = current;
        }
    }

    return result;
}

double real_maximum(const double* values, const size_t length) {
    double result = -DBL_MAX;

    for (size_t index = 0; index < length; ++index) {
        double current = values[index];

        if (current > result) {
            result = current;
        }
    }

    return result;
}

double real_mean(const double* values, const size_t length) {
    return real_sum(values, length) / length; 
}

complex_t complex_mean(const complex_t* values, const size_t length) {
    complex_t sumValue = complex_sum(values, length);
    complex_t count = from_real_to_complex(length);

    return complex_div(&sumValue, &count);
}

double real_hmean(const double* values, const size_t length) {
    double result = 0;

    for (size_t index = 0; index < length; ++index) {
        result += 1 / values[index];
    }

    return length / result;
}

complex_t complex_hmean(const complex_t* values, const size_t length) {
    complex_t result = ZERO_COMPLEX;

    for (size_t index = 0; index < length; ++index) {
        complex_t invValue = complex_inv(values + index);

        result = complex_plus(&result, &invValue);
    }

    complex_t count = from_real_to_complex(length);

    return complex_div(&count, &result);
}

double real_gmean(const double* values, const size_t length) {
    double result = 1;

    for (size_t index = 0; index < length; ++index) {
        result *= values[index];
    }

    return pow(result, 1.0 / length);
}

complex_t complex_gmean(const complex_t* values, const size_t length) {
    complex_t result = ONE_COMPLEX;

    for (size_t index = 0; index < length; ++index) {
        const complex_t* current = values + index;

        result = complex_mult(&result, current);
    }

    complex_t invCount = from_real_to_complex(1.0 / length);

    return complex_pow(&result, &invCount);
}

double real_pmean(const double* values, const size_t length, const double power) {
    if (nearly_equal(0, power)) {
        return real_gmean(values, length);
    }

    double result = 0;

    for (size_t index = 0; index < length; ++index) {
        result += pow(values[index], power);
    }

    return pow(result / length, 1 / power);
}

complex_t complex_pmean(const complex_t* values, const size_t length, const complex_t* power) {
    if (complex_equal(&ZERO_COMPLEX, power)) {
        return complex_gmean(values, length);
    }

    complex_t result = ZERO_COMPLEX;

    for (size_t index = 0; index < length; ++index) {
        const complex_t* current = values + index;

        result = complex_pow(current, power);
    }

    complex_t count = from_real_to_complex(length);
    complex_t base = complex_div(&result, &count);
    complex_t exponent = complex_inv(power);

    return complex_pow(&base, &exponent);
}

static void qselect_swap(double* values, const size_t index1, const size_t index2) {
    double temp = values[index1];
    values[index1] = values[index2];
    values[index2] = temp;
}

static size_t qselect_partition(double* values, const size_t lowIndex, const size_t highIndex) {
    double pivot = values[highIndex];
    size_t index1 = lowIndex;

    for (size_t index2 = lowIndex; index2 < highIndex; ++index2) {
        if (values[index2] <= pivot) {
            qselect_swap(values, index1, index2);
            ++index1;
        }
    }

    qselect_swap(values, index1, highIndex);

    return index1;
}

static double qselect(double* values, const size_t lowIndex, const size_t highIndex, const size_t kthIndex) {
    size_t pivotIndex = qselect_partition(values, lowIndex, highIndex);
    size_t index = pivotIndex - lowIndex;

    if (index < kthIndex) {
        return qselect(values, pivotIndex + 1, highIndex, kthIndex - pivotIndex + 1);
    }
    else if (index > kthIndex) {
        return qselect(values, lowIndex, pivotIndex + 1, kthIndex);
    }

    return values[pivotIndex];
}

static double* copy_array(const double* values, const size_t length) {
    double* copy = (double*)malloc(length * sizeof(double));
    check_alloc(copy);

    for (size_t index = 0; index < length; ++index) {
        copy[index] = values[index];
    }

    return copy;
}

double real_median(const double* values, const size_t length) {
    double* copy = copy_array(values, length);

    size_t midIndex = length / 2;
    size_t lastIndex = length - 1;

    double result = NAN;

    if (1 == length % 2) {
        result = qselect(copy, 0, lastIndex, midIndex);
    }
    else {
        double mid1 = qselect(copy, 0, lastIndex, midIndex - 1);
        double mid2 = qselect(copy, 0, lastIndex, midIndex);

        result = (mid1 + mid2) / 2;
    }

    free(copy);

    return result;
}

static double* create_modes_array(const size_t length) {
    size_t maxPossibleAmountOfModes = length / 2;

    double* modes = (double*)malloc(maxPossibleAmountOfModes * sizeof(double));
    check_alloc(modes);

    return modes;
}

static int value_comp(const void* left, const void* right) {
    double* leftPtr = (double*)left;
    double* rightPtr = (double*)right;

    if (*leftPtr < *rightPtr) {
        return -1;
    }
    else if (*leftPtr > *rightPtr) {
        return 1;
    }
    else {
        return 0;
    }
}

static size_t find_next_unequal_element(const double* sorted, const size_t length, const size_t startIndex) {
    double startValue = sorted[startIndex];
    size_t nextIndex = startIndex + 1;

    while (nextIndex < length && nearly_equal(startValue, sorted[nextIndex])) {
        ++nextIndex;
    }

    return nextIndex;
}

static real_mode_t make_mode(double* modes, const size_t modeLength, const size_t maxCount) {
    real_mode_t result;

    if (1 == maxCount) {
        result.values = NULL;
        result.length = 0;
        result.maxCount = 1;
    }
    else {
        result.values = modes;
        result.length = modeLength;
        result.maxCount = maxCount;
    }

    return result;
}

real_mode_t real_mode(const double* values, const size_t length) {
    double* copy = copy_array(values, length);
    qsort(copy, length, sizeof(double), value_comp);

    double* modes = create_modes_array(length);
    size_t modeLength = 0;

    size_t maxCount = 1;
    size_t startIndex = 0;
    
    while (startIndex < length) {
        size_t nextIndex = find_next_unequal_element(copy, length, startIndex);
        size_t currentCount = nextIndex - startIndex;
        
        if (currentCount == maxCount) {
            modes[modeLength] = copy[startIndex];
            ++modeLength;
        }
        else if (currentCount > maxCount) {
            maxCount = currentCount;

            modes[0] = copy[startIndex];
            modeLength = 1;
        }

        startIndex = nextIndex;
    }
    
    free(copy);

    return make_mode(modes, modeLength, maxCount);
}

double real_range(const double* values, const size_t length) {
    return real_maximum(values, length) - real_minimum(values, length);
}

double real_midrange(const double* values, const size_t length) {
    return real_range(values, length) / 2;
}

double real_variance(const double* values, const size_t length) {
    double result = 0;
    double meanValue = real_mean(values, length);

    for (size_t index = 0; index < length; ++index) {
        double val = values[index] - meanValue;

        result += val * val;
    }

    return result / length;
}

complex_t complex_variance(const complex_t* values, const size_t length) {
    complex_t result = ZERO_COMPLEX;
    complex_t meanValue = complex_mean(values, length);

    for (size_t index = 0; index < length; ++index) {
        const complex_t* current = values + index;
        complex_t val1 = complex_minus(current, &meanValue);
        complex_t val2 = complex_mult(&val1, &val1);

        result = complex_plus(&result, &val2);
    }

    complex_t count = from_real_to_complex(length);

    return complex_div(&result, &count);
}

double real_stddev(const double* values, const size_t length) {
    return sqrt(real_variance(values, length));
}

complex_t complex_stddev(const complex_t* values, const size_t length) {
    complex_t varianceValue = complex_variance(values, length);

    return complex_sqrt(&varianceValue);
}
