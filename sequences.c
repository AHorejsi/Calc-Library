#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "alloc.h"
#include "digits.h"
#include "sequences.h"


typedef double(*next_item_t)(const double, const double);

uint64_t fibonacci_elements(const uint32_t index) {
    const double GOLDEN_RATIO = (1 + sqrt(5)) / 2;

    double a = pow(GOLDEN_RATIO, index);
    double b = pow(1 - GOLDEN_RATIO, index);
    double result = (a - b) / sqrt(5);

    return (uint64_t)result;
}

double arithmetic_element(const num_seq_t* seq, const uint32_t index) {
    return seq->initial + index * seq->common;
}

double geometric_element(const num_seq_t* seq, const uint32_t index) {
    return seq->initial * pow(seq->common, index);
}

double harmonic_element(const num_seq_t* seq, const uint32_t index) {
    return 1 / arithmetic_element(seq, index);
}

uint64_t* fibonacci_list(const size_t count) {
    if (0 == count) {
        return NULL;
    }

    uint64_t* elements = (uint64_t*)malloc(count * sizeof(uint64_t));
    check_alloc(elements);

    if (1 == count) {
        *elements = 0;

        return elements;
    }

    uint64_t first = 0;
    uint64_t second = 1;
    bool useFirst = true;

    uint64_t* ptr = elements;

    *(ptr++) = first;
    *(ptr++) = second;

    for (size_t index = 2; index < count; ++index) {
        uint64_t sum = first + second;

        *(ptr++) = sum;

        if (useFirst) {
            first = sum;
        }
        else {
            second = sum;
        }

        useFirst = !useFirst;
    }

    return elements;
}

void create_list(const num_seq_t* seq, const size_t count, double* ptr, const next_item_t next) {
    double item = seq->initial;
    double adjust = seq->common;

    for (size_t index = 0; index < count; ++index) {
        *ptr = item;
        ++ptr;

        item = next(item, adjust);
    }
}

static double next_arithmetic(const double current, const double diff) {
    return current + diff;
}

double* arithmetic_list(const num_seq_t* seq, const size_t count) {
    double* elements = (double*)malloc(count * sizeof(double));
    check_alloc(elements);

    create_list(seq, count, elements, next_arithmetic);

    return elements;
}

static double next_geometric(const double current, const double ratio) {
    return current * ratio;
}

double* geometric_list(const num_seq_t* seq, const size_t count) {
    double* elements = (double*)malloc(count * sizeof(double));
    check_alloc(elements);

    create_list(seq, count, elements, next_geometric);

    return elements;
}

double* harmonic_list(const num_seq_t* seq, const size_t count) {
    double* elements = (double*)malloc(count * sizeof(double));
    check_alloc(elements);

    double* ptr = elements;
    double item = seq->initial;
    double diff = seq->common;

    for (size_t index = 0; index < count; ++index) {
        *ptr = 1 / item;
        ++ptr;

        item += diff;
    }

    return elements;
}
