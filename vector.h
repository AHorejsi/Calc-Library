#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "complex.h"


typedef struct {
    double xAxis;
    double yAxis;
} vector_2d_t;

typedef struct {
    double xAxis;
    double yAxis;
    double zAxis;
} vector_3d_t;

typedef struct {
    double wAxis;
    double xAxis;
    double yAxis;
    double zAxis;
} vector_4d_t;

typedef struct {
    double* values;
    size_t dimensions;
} real_vector_t;

typedef struct {
    complex_t* values;
    size_t dimensions;
} complex_vector_t;

extern const vector_2d_t INVALID_VEC_2D;
extern const vector_3d_t INVALID_VEC_3D;
extern const vector_4d_t INVALID_VEC_4D;
extern const real_vector_t INVALID_REAL_VEC;
extern const complex_vector_t INVALID_COMPLEX_VEC;

real_vector_t to_2d_real_vec(const vector_2d_t*);

real_vector_t to_3d_real_vec(const vector_3d_t*);

real_vector_t to_4d_real_vec(const vector_4d_t*);

void destroy_real_vector(real_vector_t*);

void destroy_complex_vector(complex_vector_t*);

bool isnan_vec_2d(const vector_2d_t*);

bool isnan_vec_3d(const vector_3d_t*);

bool isnan_vec_4d(const vector_4d_t*);

bool isnan_real_vector(const real_vector_t*);

bool isnan_complex_vector(const complex_vector_t*);

bool is_null_vec_2d(const vector_2d_t*);

bool is_null_vec_3d(const vector_3d_t*);

bool is_null_vec_4d(const vector_4d_t*);

bool is_null_real_vector(const real_vector_t*);

bool is_null_complex_vector(const complex_vector_t*);

vector_2d_t vec_2d_plus(const vector_2d_t*, const vector_2d_t*);

vector_3d_t vec_3d_plus(const vector_3d_t*, const vector_3d_t*);

vector_4d_t vec_4d_plus(const vector_4d_t*, const vector_4d_t*);

real_vector_t real_vector_plus(const real_vector_t*, const real_vector_t*);

complex_vector_t complex_vector_plus(const complex_vector_t*, const complex_vector_t*);

vector_2d_t vec_2d_neg(const vector_2d_t*);

vector_3d_t vec_3d_neg(const vector_3d_t*);

vector_4d_t vec_4d_neg(const vector_4d_t*);

real_vector_t real_vec_neg(const real_vector_t*);

complex_vector_t complex_vec_neg(const complex_vector_t*);

vector_2d_t vec_2d_minus(const vector_2d_t*, const vector_2d_t*);

vector_3d_t vec_3d_minus(const vector_3d_t*, const vector_3d_t*);

vector_4d_t vec_4d_minus(const vector_4d_t*, const vector_4d_t*);

real_vector_t real_vec_minus(const real_vector_t*, const real_vector_t*);

complex_vector_t complex_vec_minus(const complex_vector_t*, const complex_vector_t*);

vector_2d_t vec_2d_mult(const vector_2d_t*, const double);

vector_3d_t vec_3d_mult(const vector_3d_t*, const double);

vector_4d_t vec_4d_mult(const vector_4d_t*, const double);

real_vector_t real_vec_mult(const real_vector_t*, const double);

complex_vector_t complex_vec_mult(const complex_vector_t*, const complex_t*);

vector_2d_t vec_2d_div(const vector_2d_t*, const double);

vector_3d_t vec_3d_div(const vector_3d_t*, const double);

vector_4d_t vec_4d_div(const vector_4d_t*, const double);

real_vector_t real_vec_div(const real_vector_t*, const double);

complex_vector_t complex_vec_div(const complex_vector_t*, const complex_t*);

double vec_2d_dot(const vector_2d_t*, const vector_2d_t*);

double vec_3d_dot(const vector_3d_t*, const vector_3d_t*);

double vec_4d_dot(const vector_4d_t*, const vector_4d_t*);

double real_vec_dot(const real_vector_t*, const real_vector_t*);

complex_t complex_vec_dot(const complex_vector_t*, const complex_vector_t*);

vector_3d_t vec_3d_cross(const vector_3d_t*, const vector_3d_t*);

vector_2d_t vec_2d_scale(const vector_2d_t*, const vector_2d_t*);

vector_3d_t vec_3d_scale(const vector_3d_t*, const vector_3d_t*);

vector_4d_t vec_4d_scale(const vector_4d_t*, const vector_4d_t*);

real_vector_t real_vec_scale(const real_vector_t*, const real_vector_t*);

complex_vector_t complex_vec_scale(const complex_vector_t*, const complex_vector_t*);

double vec_2d_magn(const vector_2d_t*);

double vec_3d_magn(const vector_3d_t*);

double vec_4d_magn(const vector_4d_t*);

double real_vec_magn(const real_vector_t*);

complex_t complex_vec_magn(const complex_vector_t*);

vector_2d_t vec_2d_norm(const vector_2d_t*);

vector_3d_t vec_3d_norm(const vector_3d_t*);

vector_4d_t vec_4d_norm(const vector_4d_t*);

real_vector_t real_vec_norm(const real_vector_t*);

complex_vector_t complex_vec_norm(const complex_vector_t*);

double vec_2d_angle(const vector_2d_t*, const vector_2d_t*);

double vec_3d_angle(const vector_3d_t*, const vector_3d_t*);

double vec_4d_angle(const vector_4d_t*, const vector_4d_t*);

double real_vec_angle(const real_vector_t*, const real_vector_t*);

complex_t complex_vec_angle(const complex_vector_t*, const complex_vector_t*);


#endif
