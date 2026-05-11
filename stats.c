#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include "alloc.h"
#include "floating_points.h"
#include "stats.h"


const real_list_t EMPTY_REAL_LIST = { NULL, 0 };
const complex_list_t EMPTY_COMPLEX_LIST = { NULL, 0 };

void destroy_real_list(real_list_t* list) {
    free(list->nums);
}

void destroy_complex_list(complex_list_t* list) {
    free(list->nums);
}

double real_sum(const real_list_t* list) {
    double* ptr = list->nums;
    size_t count = list->count;

    double result = 0;

    for (size_t index = 0; index < count; ++index) {
        result += (*ptr);
        ++ptr;
    }

    return result;
}

complex_t complex_sum(const complex_list_t* list) {
    complex_t* ptr = list->nums;
    size_t count = list->count;

    complex_t result = ZERO_COMPLEX;

    for (size_t index = 0; index < count; ++index) {
        result = complex_plus(&result, ptr);
        ++ptr;
    }

    return result;
}

real_list_t real_cumsum(const real_list_t* list) {
    if (0 == list->count) {
        return EMPTY_REAL_LIST;
    }

    size_t count = list->count;
    double* newNums = (double*)malloc(count * sizeof(double));
    check_alloc(newNums);

    double* oldPtr = list->nums;
    double* newPtr = newNums;
    double current = 0;

    for (size_t index = 0; index < count; ++index) {
        current += (*oldPtr);
        *newPtr = current;

        ++oldPtr;
        ++newPtr;
    }

    real_list_t result = { newNums, count };
    
    return result;
}

complex_list_t complex_cumsum(const complex_list_t* list) {
    if (0 == list->count) {
        return EMPTY_COMPLEX_LIST;
    }

    size_t count = list->count;
    complex_t* newNums = (complex_t*)malloc(count * sizeof(complex_t));
    check_alloc(newNums);

    complex_t* oldPtr = list->nums;
    complex_t* newPtr = newNums;
    complex_t current = ZERO_COMPLEX;

    for (size_t index = 0; index < count; ++index) {
        current = complex_plus(&current, oldPtr);
        *newPtr = current;

        ++oldPtr;
        ++newPtr;
    }

    complex_list_t result = { newNums, count };

    return result;
}

double real_prod(const real_list_t* list) {
    double* ptr = list->nums;
    size_t count = list->count;

    double result = 1;

    for (size_t index = 0; index < count; ++index) {
        result *= (*ptr);
        ++ptr;
    }

    return result;
}

complex_t complex_prod(const complex_list_t* list) {
    complex_t* ptr = list->nums;
    size_t count = list->count;

    complex_t result = ONE_COMPLEX;

    for (size_t index = 0; index < count; ++index) {
        result = complex_mult(&result, ptr);
        ++ptr;
    }

    return result;
}

real_list_t real_cumprod(const real_list_t* list) {
    if (0 == list->count) {
        return EMPTY_REAL_LIST;
    }

    size_t count = list->count;
    double* newNums = (double*)malloc(count * sizeof(double));
    check_alloc(newNums);

    double* oldPtr = list->nums;
    double* newPtr = newNums;
    double current = 1;

    for (size_t index = 0; index < count; ++index) {
        current += (*oldPtr);
        *newPtr = current;

        ++oldPtr;
        ++newPtr;
    }

    real_list_t result = { newNums, count };
    
    return result;
}

complex_list_t complex_cumprod(const complex_list_t* list) {
    if (0 == list->count) {
        return EMPTY_COMPLEX_LIST;
    }

    size_t count = list->count;
    complex_t* newNums = (complex_t*)malloc(count * sizeof(complex_t));
    check_alloc(newNums);

    complex_t* oldPtr = list->nums;
    complex_t* newPtr = newNums;
    complex_t current = ONE_COMPLEX;

    for (size_t index = 0; index < count; ++index) {
        current = complex_mult(&current, oldPtr);
        *newPtr = current;

        ++oldPtr;
        ++newPtr;
    }

    complex_list_t result = { newNums, count };

    return result;
}

double real_minimum(const real_list_t* list) {
    size_t count = list->count;
    double* ptr = list->nums;

    double result = DBL_MAX;

    for (size_t index = 0; index < count; ++index) {
        double current = *ptr;

        if (current < result) {
            result = current;
        }

        ++ptr;
    }

    return result;
}

double real_maximum(const real_list_t* list) {
    size_t count = list->count;
    double* ptr = list->nums;

    double result = DBL_MAX;

    for (size_t index = 0; index < count; ++index) {
        double current = *ptr;

        if (current > result) {
            result = current;
        }

        ++ptr;
    }

    return result;
}

double real_mean(const real_list_t* list) {
    return real_sum(list) / list->count;
}

complex_t complex_mean(const complex_list_t* list) {
    complex_t sum = complex_sum(list);
    complex_t count = from_real(list->count);

    return complex_div(&sum, &count);
}

double real_hmean(const real_list_t* list) {
    size_t count = list->count;
    double* ptr = list->nums;

    double result = 0;

    for (size_t index = 0; index < count; ++index) {
        result += 1 / (*ptr);
        ++ptr;
    }

    return count / result;
}

complex_t complex_hmean(const complex_list_t* list) {
    size_t count = list->count;
    complex_t* ptr = list->nums;

    complex_t result = ZERO_COMPLEX;

    for (size_t index = 0; index < count; ++index) {
        complex_t inv = complex_inv(ptr);
        result = complex_plus(&result, &inv);

        ++ptr;
    }

    complex_t count = from_real(list->count);

    return complex_div(&count, &result);
}

complex_t complex_hmean(const complex_t* values, const size_t length) {
    complex_t result = ZERO_COMPLEX;

    for (size_t index = 0; index < length; ++index) {
        complex_t invValue = complex_inv(values + index);

        result = complex_plus(&result, &invValue);
    }

    complex_t count = from_real(length);

    return complex_div(&count, &result);
}

double real_gmean(const real_list_t* list) {
    double prod = real_prod(list);
    double countInv = 1.0 / list->count;

    return pow(prod, countInv);
}

complex_t complex_gmean(const complex_list_t* list) {
    complex_t prod = complex_prod(list);
    complex_t countInv = from_real(1.0 / list->count);

    return complex_pow(&prod, &countInv);
}

double real_pmean(const real_list_t* list, const double power) {
    if (nearly_equal(0, power)) {
        return real_gmean(list);
    }

    size_t count = list->count;
    double* ptr = list->nums;

    double result = 0;

    for (size_t index = 0; index < count; ++index) {
        result += pow(*ptr, power);
        ++ptr;
    }

    return pow(result / count, 1 / power);
}

complex_t complex_pmean(const complex_list_t* list, const complex_t* power) {
    if (complex_equal(&ZERO_COMPLEX, power)) {
        return complex_gmean(list);
    }

    size_t count = list->count;
    complex_t* ptr = list->nums;

    complex_t result = ZERO_COMPLEX;

    for (size_t index = 0; index < count; ++index) {
        complex_t current = complex_pow(ptr, power);
        result = complex_plus(&result, &current);

        ++ptr;
    }
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

static double qselect(double* values, const size_t lowIndex, const size_t highIndex, const size_t nthIndex) {
    size_t pivotIndex = qselect_partition(values, lowIndex, highIndex);
    size_t index = pivotIndex - lowIndex;

    if (index < nthIndex) {
        return qselect(values, pivotIndex + 1, highIndex, nthIndex - pivotIndex + 1);
    }
    else if (index > nthIndex) {
        return qselect(values, lowIndex, pivotIndex + 1, nthIndex);
    }

    return values[pivotIndex];
}

static double* copy_array(const real_list_t* list) {
    size_t count = list->count;

    double* copy = (double*)malloc(count * sizeof(double));
    check_alloc(copy);

    double* oldPtr = list->nums;
    double* newPtr = copy;

    for (size_t index = 0; index < count; ++index) {
        *newPtr = *oldPtr;

        ++oldPtr;
        ++newPtr;
    }

    return copy;
}

double real_median(const real_list_t* list) {
    size_t count = list->count;

    double* copy = copy_array(list);
    size_t midIndex = count / 2;

    double result;

    if (1 == count % 2) {
        result = qselect(copy, 0, count - 1, midIndex);
    }
    else {
        size_t lastIndex = count - 1;

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

real_mode_t real_mode(const real_list_t* list) {
    size_t count = list->count;

    double* copy = copy_array(list);
    qsort(copy, count, sizeof(double), value_comp);

    double* modes = create_modes_array(count);
    size_t modeLength = 0;

    size_t maxCount = 1;
    size_t startIndex = 0;

    while (startIndex < count) {
        size_t nextIndex = find_next_unequal_element(copy, count, startIndex);
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

double real_range(const real_list_t* list) {
    return real_maximum(list) - real_minimum(list);
}

double real_midrange(const real_list_t* list) {
    return real_range(list) / 2;
}

double real_variance(const real_list_t* list) {
    size_t count = list->count;
    double* ptr = list->nums;

    double mean = real_mean(list);
    double result = 0;

    for (size_t index = 0; index < count; ++index) {
        double elem = (*ptr) - mean;

        result += elem * elem;

        ++ptr;
    }

    return result / count;
}

complex_t complex_variance(const complex_list_t* list) {
    size_t count = list->count;
    complex_t* ptr = list->nums;

    complex_t mean = complex_mean(list);
    complex_t result = ZERO_COMPLEX;

    for (size_t index = 0; index < count; ++index) {
        complex_t elem = complex_minus(ptr, &mean);
        complex_t square = complex_mult(&elem, &elem);

        result = complex_plus(&result, &square);
    }

    complex_t comCount = from_real(count);

    return complex_div(&result, &comCount);
}

double real_stddev(const real_list_t* list) {
    return sqrt(real_variance(list));
}

complex_t complex_stddev(const complex_list_t* list) {
    complex_t var = complex_variance(list);

    return complex_sqrt(&var);
}

double real_select(const real_list_t* list, const size_t nth) {
    return qselect(list->nums, 0, list->count - 1, nth);
}

void real_sort(const real_list_t* list) {
    qsort(list->nums, list->count, sizeof(double), value_comp);
}
