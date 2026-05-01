#ifndef SEQUENCES_H
#define SEQUENCES_H

#include <stdint.h>


typedef struct {
    double initial;
    double common;
} num_seq_t;

uint64_t fibonacci_elements(const uint32_t);

double arithmetic_element(const num_seq_t*, const uint32_t);

double geometric_element(const num_seq_t*, const uint32_t);

double harmonic_element(const num_seq_t*, const uint32_t);

uint64_t* fibonacci_list(const size_t);

double* arithmetic_list(const num_seq_t*, const size_t);

double* geometric_list(const num_seq_t*, const size_t);

double* harmonic_list(const num_seq_t*, const size_t);


#endif
