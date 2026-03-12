#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include "alloc.h"
#include "floating_points.h"
#include "stats.h"


double sum(const double* values, const size_t length) {
    double sumValue = 0;

    for (size_t index = 0; index < length; ++index) {
        sumValue += values[index];
    }

    return sumValue;
}

double* cumsum(const double* values, const size_t length) {
    double* result = (double*)malloc(length * sizeof(double));
    check_alloc(result);

    result[0] = values[0];
    for (size_t index = 1; index < length; ++index) {
        result[index] = values[index] + result[index - 1];
    }

    return result;
}

double prod(const double* values, const size_t length) {
    double prodValue = 1;

    for (size_t index = 0; index < length; ++index) {
        prodValue *= values[index];
    }

    return prodValue;
}

double* cumprod(const double* values, const size_t length) {
    double* result = (double*)malloc(length * sizeof(double));
    check_alloc(result);

    result[0] = values[0];
    for (size_t index = 1; index < length; ++index) {
        result[index] = values[index] * result[index - 1];
    }

    return result;
}

double minimum(const double* values, const size_t length) {
    double result = DBL_MAX;

    for (size_t index = 0; index < length; ++index) {
        double current = values[index];

        if (current < result) {
            result = current;
        }
    }

    return result;
}

double maximum(const double* values, const size_t length) {
    double result = -DBL_MAX;

    for (size_t index = 0; index < length; ++index) {
        double current = values[index];

        if (current > result) {
            result = current;
        }
    }

    return result;
}

double mean(const double* values, const size_t length) {
    return sum(values, length) / length; 
}

double hmean(const double* values, const size_t length) {
    double result = 0;

    for (size_t index = 0; index < length; ++index) {
        result += 1 / values[index];
    }

    return length / result;
}

double gmean(const double* values, const size_t length) {
    double result = 1;

    for (size_t index = 0; index < length; ++index) {
        result *= values[index];
    }

    return pow(result, 1.0 / length);
}

double pmean(const double* values, const size_t length, const double power) {
    if (nearly_equal(0, power)) {
        return gmean(values, length);
    }

    double result = 0;

    for (size_t index = 0; index < length; ++index) {
        result += pow(values[index], power);
    }

    return pow(result / length, 1 / power);
}

static void qselect_swap(double* values, const size_t index1, const size_t index2) {
    double temp = values[index1];
    values[index1] = values[index2];
    values[index2] = temp;
}

static size_t qselect_partition(double* values, const size_t length, const size_t lowIndex, const size_t highIndex) {
    double pivot = values[highIndex];
    size_t index1 = lowIndex;

    for (size_t index2 = lowIndex; index2 < highIndex; ++index2) {
        if (values[index2] <= pivot) {
            qselect_swap(values, index1++, index2);
        }
    }

    qselect_swap(values, index1, highIndex);

    return index1;
}

static double qselect(double* values, const size_t length, const size_t lowIndex, const size_t highIndex, const size_t kthIndex) {
    size_t pivotIndex = qselect_partition(values, length, lowIndex, highIndex);
    size_t index = pivotIndex - lowIndex;

    if (index < kthIndex) {
        return qselect(values, length, pivotIndex + 1, highIndex, kthIndex - pivotIndex + 1);
    }
    else if (index > kthIndex) {
        return qselect(values, length, lowIndex, pivotIndex + 1, kthIndex);
    }

    return values[pivotIndex];
}

static double* copy_array(const double* values, const size_t length) {
    double byteSize = length * sizeof(double);

    double* copy = (double*)malloc(byteSize);
    check_alloc(copy);

    memcpy(copy, values, byteSize);

    return copy;
}

double median(const double* values, const size_t length) {
    double* copy = copy_array(values, length);

    size_t midIndex = length / 2;
    double result;

    if (1 == length % 2) {
        result = qselect(copy, length, 0, length - 1, midIndex);
    }
    else {
        double mid1 = qselect(copy, length, 0, length - 1, midIndex - 1);
        double mid2 = qselect(copy, length, 0, length - 1, midIndex);

        result = (mid1 + mid2) / 2;
    }

    free(copy);

    return result;
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

static mode_t make_mode(double* modes, const size_t modeLength, const size_t maxCount) {
    mode_t result;

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

mode_t mode(const double* values, const size_t length) {
    double* copy = copy_array(values, length);
    qsort(copy, length, sizeof(double), value_comp);

    size_t maxPossibleAmountOfModes = length / 2;
    double* modes = (double*)malloc(maxPossibleAmountOfModes * sizeof(double));

    size_t modeLength = 0;

    size_t maxCount = 1;
    size_t startIndex = 0;
    
    while (startIndex < length) {
        size_t nextIndex = find_next_unequal_element(copy, length, startIndex);
        size_t currentCount = nextIndex - startIndex;
        
        if (currentCount > maxCount) {
            maxCount = currentCount;

            modes[0] = copy[startIndex];
            modeLength = 1;
        }
        else if (currentCount == maxCount) {
            maxCount = 1;

            modes[modeLength] = copy[startIndex];
            ++modeLength;
        }

        startIndex = nextIndex;
    }
    
    free(copy);

    return make_mode(modes, modeLength, maxCount);
}

double range(const double* values, const size_t length) {
    return maximum(values, length) - minimum(values, length);
}

double midrange(const double* values, const size_t length) {
    return range(values, length) / 2;
}

double variance(const double* values, const size_t length) {
    double result = 0;
    double meanValue = mean(values, length);

    for (size_t index = 0; index < length; ++index) {
        double val = values[index] - meanValue;

        result += val * val;
    }

    return result / length;
}

double stddev(const double* values, const size_t length) {
    return sqrt(variance(values, length));
}
