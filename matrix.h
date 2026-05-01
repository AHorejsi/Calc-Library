#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "complex.h"
#include "quaternion.h"
#include "vector.h"


typedef struct {
    double* values;
    size_t rows;
    size_t cols;
} real_matrix_t;

typedef struct {
    complex_t* values;
    size_t rows;
    size_t cols;
} complex_matrix_t;

extern const real_matrix_t INVALID_REAL_MATRIX;
extern const complex_matrix_t INVALID_COMPLEX_MATRIX;

void destroy_real_matrix(real_matrix_t*);

void destroy_complex_matrix(complex_matrix_t*);

double real_matrix_row_get(const real_matrix_t*, const size_t, const size_t);

complex_t complex_matrix_row_get(const complex_matrix_t*, const size_t, const size_t);

double real_matrix_col_get(const real_matrix_t*, const size_t, const size_t);

complex_t complex_matrix_col_get(const complex_matrix_t*, const size_t, const size_t);

void real_matrix_row_set(real_matrix_t*, const size_t, const size_t, const double);

void complex_matrix_row_set(complex_matrix_t*, const size_t, const size_t, const complex_t);

void real_matrix_col_set(real_matrix_t*, const size_t, const size_t, const double);

void complex_matrix_col_set(complex_matrix_t*, const size_t, const size_t, const complex_t);

bool is_real_square(const real_matrix_t*);

bool is_complex_square(const complex_matrix_t*);

bool are_real_addable(const real_matrix_t*, const real_matrix_t*);

bool are_complex_addable(const complex_matrix_t*, const complex_matrix_t*);

bool are_real_multipliable(const real_matrix_t*, const real_matrix_t*);

bool are_complex_multipliable(const complex_matrix_t*, const complex_matrix_t*);

real_matrix_t from_complex_to_real_matrix(const complex_t*);

real_matrix_t from_quaternion_to_real_matrix(const quaternion_t*);

complex_matrix_t from_quaternion_to_complex_matrix(const quaternion_t*);

real_matrix_t real_row_matrix(const real_vector_t*);

real_matrix_t real_col_matrix(const real_vector_t*);

complex_matrix_t complex_row_matrix(const complex_vector_t*);

complex_matrix_t complex_col_matrix(const complex_vector_t*);

size_t actual_length_real(const real_matrix_t*);

size_t actual_length_complex(const complex_matrix_t*);

real_matrix_t init_real_matrix(const size_t, const size_t);

complex_matrix_t init_complex_matrix(const size_t, const size_t);

real_matrix_t zero_real_matrix(const size_t, const size_t);

complex_matrix_t zero_complex_matrix(const size_t, const size_t);

real_matrix_t identity_real_matrix(const size_t);

complex_matrix_t identity_complex_matrix(const size_t);

real_matrix_t fill_real_matrix(const size_t, const size_t, const double);

complex_matrix_t fill_complex_matrix(const size_t, const size_t, const complex_t);

real_matrix_t real_matrix_plus(const real_matrix_t*, const real_matrix_t*);

complex_matrix_t complex_matrix_plus(const complex_matrix_t*, const complex_matrix_t*);

real_matrix_t real_matrix_negate(const real_matrix_t*);

complex_matrix_t complex_matrix_negate(const complex_matrix_t*);

real_matrix_t real_matrix_minus(const real_matrix_t*, const real_matrix_t*);

complex_matrix_t complex_matrix_minus(const complex_matrix_t*, const complex_matrix_t*);

real_matrix_t real_matrix_mult(const real_matrix_t*, const real_matrix_t*);

complex_matrix_t complex_matrix_mult(const complex_matrix_t*, const complex_matrix_t*);

real_matrix_t real_matrix_scalar_mult(const real_matrix_t*, const double);

complex_matrix_t complex_matrix_scalar_mult(const complex_matrix_t*, const complex_t*);

real_matrix_t real_matrix_scale(const real_matrix_t*, const real_matrix_t*);

complex_matrix_t complex_matrix_scale(const complex_matrix_t*, const complex_matrix_t*);

real_matrix_t real_matrix_mult_real_vec(const real_matrix_t*, const real_vector_t*);

complex_matrix_t complex_matrix_mult_complex_vec(const complex_matrix_t*, const complex_vector_t*);

real_matrix_t real_vec_mult_real_matrix(const real_vector_t*, const real_matrix_t*);

complex_matrix_t complex_vec_mult_complex_matrix(const complex_vector_t*, const complex_matrix_t*);

double real_matrix_determinant(const real_matrix_t*);

complex_t complex_matrix_determinant(const complex_matrix_t*);

real_matrix_t real_matrix_inv(const real_matrix_t*);

complex_matrix_t complex_matrix_inv(const complex_matrix_t*);

real_matrix_t real_matrix_div(const real_matrix_t*, const real_matrix_t*);

complex_matrix_t complex_matrix_div(const complex_matrix_t*, const complex_matrix_t*);

real_matrix_t real_matrix_scalar_div(const real_matrix_t*, const double);

complex_matrix_t complex_matrix_scalar_div(const complex_matrix_t*, const complex_t*);

real_matrix_t real_matrix_transpose(const real_matrix_t*);

complex_matrix_t complex_matrix_transpose(const complex_matrix_t*);

real_matrix_t sub_real_matrix(const real_matrix_t*, const size_t, const size_t);

real_matrix_t row_add_real(const real_matrix_t*, const size_t, const size_t);

complex_matrix_t row_add_complex(const complex_matrix_t*, const size_t, const size_t);

real_matrix_t row_mult_real(const real_matrix_t*, const double, const size_t);

complex_matrix_t row_mult_complex(const complex_matrix_t*, const complex_t*, const size_t);

real_matrix_t row_swap_real(const real_matrix_t*, const size_t, const size_t);

complex_matrix_t row_swap_complex(const complex_matrix_t*, const size_t, const size_t);

real_matrix_t col_add_real(const real_matrix_t*, const size_t, const size_t);

complex_matrix_t col_add_complex(const complex_matrix_t*, const size_t, const size_t);

real_matrix_t col_mult_real(const real_matrix_t*, const double, const size_t);

complex_matrix_t col_mult_complex(const complex_matrix_t*, const complex_t*, const size_t);

real_matrix_t col_swap_real(const real_matrix_t*, const size_t, const size_t);

complex_matrix_t col_swap_complex(const complex_matrix_t*, const size_t, const size_t);


#endif
