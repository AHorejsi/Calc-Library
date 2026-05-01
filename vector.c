#include <math.h>
#include <stdlib.h>
#include "floating_points.h"
#include "alloc.h"
#include "vector.h"


const vector_2d_t INVALID_VEC_2D = { NAN, NAN };
const vector_3d_t INVALID_VEC_3D = { NAN, NAN, NAN };
const vector_4d_t INVALID_VEC_4D = { NAN, NAN, NAN, NAN };
const real_vector_t INVALID_REAL_VEC = { NULL, 0 };
const complex_vector_t INVALID_COMPLEX_VEC = { NULL, 0 };

real_vector_t to_2d_real_vec(const vector_2d_t* vec) {
    const size_t DIMENSIONS = 2;

    double* values = (double*)malloc(DIMENSIONS * sizeof(double));
    check_alloc(values);

    values[0] = vec->xAxis;
    values[1] = vec->yAxis;

    real_vector_t newVec = { values, DIMENSIONS };

    return newVec;
}

real_vector_t to_3d_real_vec(const vector_3d_t* vec) {
    const size_t DIMENSIONS = 3;

    double* values = (double*)malloc(DIMENSIONS * sizeof(double));
    check_alloc(values);

    values[0] = vec->xAxis;
    values[1] = vec->yAxis;
    values[2] = vec->zAxis;

    real_vector_t newVec = { values, DIMENSIONS };

    return newVec;
}

real_vector_t to_4d_real_vec(const vector_4d_t* vec) {
    const size_t DIMENSIONS = 4;

    double* values = (double*)malloc(DIMENSIONS * sizeof(double));
    check_alloc(values);

    values[0] = vec->wAxis;
    values[1] = vec->xAxis;
    values[2] = vec->yAxis;
    values[3] = vec->zAxis;

    real_vector_t newVec = { values, DIMENSIONS };

    return newVec;
}

void destroy_real_vector(real_vector_t* vec) {
    free(vec->values);
}

void destroy_complex_vector(complex_vector_t* vec) {
    free(vec->values);
}

bool isnan_vec_2d(const vector_2d_t* vec) {
    return isnan(vec->xAxis) && isnan(vec->yAxis);
}

bool isnan_vec_3d(const vector_3d_t* vec) {
    return isnan(vec->xAxis) && isnan(vec->yAxis) && isnan(vec->zAxis);
}

bool isnan_vec_4d(const vector_4d_t* vec) {
    return isnan(vec->wAxis) && isnan(vec->xAxis) && isnan(vec->yAxis) && isnan(vec->zAxis);
}

bool isnan_real_vector(const real_vector_t* vec) {
    for (size_t index = 0; index < vec->dimensions; ++index) {
        double val = vec->values[index];

        if (!isnan(val)) {
            return false;
        }
    }

    return true;
}

bool isnan_complex_vector(const complex_vector_t* vec) {
    complex_t* ptr = vec->values;

    for (size_t index = 0; index < vec->dimensions; ++index) {
        if (!complex_isnan(ptr)) {
            return false;
        }

        ++ptr;
    }

    return true;
}

bool is_null_vec_2d(const vector_2d_t* vec) {
    return nearly_equal(0, vec->xAxis) && nearly_equal(0, vec->yAxis);
}

bool is_null_vec_3d(const vector_3d_t* vec) {
    return nearly_equal(0, vec->xAxis) && nearly_equal(0, vec->yAxis) && nearly_equal(0, vec->zAxis);
}

bool is_null_vec_4d(const vector_4d_t* vec) {
    return nearly_equal(0, vec->wAxis) && nearly_equal(0, vec->xAxis) && nearly_equal(0, vec->yAxis) && nearly_equal(0, vec->zAxis);
}

bool is_null_real_vector(const real_vector_t* vec) {
    for (size_t index = 0; index < vec->dimensions; ++index) {
        double axis = vec->values[index];

        if (!nearly_equal(0, axis)) {
            return false;
        }
    }

    return true;
}

bool is_null_complex_vector(const complex_vector_t* vec) {
    complex_t* ptr = vec->values;

    for (size_t index = 0; index < vec->dimensions; ++index) {
        if (!complex_equal(&ZERO_COMPLEX, ptr)) {
            return false;
        }

        ++ptr;
    }

    return true;
}

vector_2d_t vec_2d_plus(const vector_2d_t* left, const vector_2d_t* right) {
    double newXAxis = left->xAxis + right->xAxis;
    double newYAxis = left->yAxis + right->yAxis;

    vector_2d_t newVec = { newXAxis, newYAxis };

    return newVec;
}

vector_3d_t vec_3d_plus(const vector_3d_t* left, const vector_3d_t* right) {
    double newXAxis = left->xAxis + right->xAxis;
    double newYAxis = left->yAxis + right->yAxis;
    double newZAxis = left->zAxis + right->zAxis;

    vector_3d_t newVec = { newXAxis, newYAxis, newZAxis };

    return newVec;
}

vector_4d_t vec_4d_plus(const vector_4d_t* left, const vector_4d_t* right) {
    double newWAxis = left->wAxis + right->wAxis;
    double newXAxis = left->xAxis + right->xAxis;
    double newYAxis = left->yAxis + right->yAxis;
    double newZAxis = left->zAxis + right->zAxis;

    vector_4d_t newVec = { newWAxis, newXAxis, newYAxis, newZAxis };

    return newVec;
}

static void linear_binary_operation(void* newValues, void* leftValues, void* rightValues, const size_t dimensions, const size_t byteSize, void(*op)(void*, const void*, const void*)) {
    byte_t* newByte = (byte_t*)newValues;
    byte_t* leftByte = (byte_t*)leftValues;
    byte_t* rightByte = (byte_t*)rightValues;

    for (size_t index = 0; index < dimensions; ++index) {
        op(newByte, leftByte, rightByte);

        newByte += byteSize;
        leftByte += byteSize;
        rightByte += byteSize;
    }
}

static void re_plus(void* ptr, const void* leftPtr, const void* rightPtr) {
    double* result = (double*)ptr;
    const double* leftReal = (double*)leftPtr;
    const double* rightReal = (double*)rightPtr;

    *result = (*leftReal) + (*rightReal);
}

real_vector_t real_vector_plus(const real_vector_t* left, const real_vector_t* right) {
    if (left->dimensions != right->dimensions) {
        return INVALID_REAL_VEC;
    }

    size_t newDimensions = left->dimensions;
    double* newValues = (double*)malloc(newDimensions * sizeof(double));
    check_alloc(newValues);

    linear_binary_operation(newValues, left->values, right->values, newDimensions, sizeof(double), re_plus);

    real_vector_t newVec = { newValues, newDimensions };

    return newVec;
}

static void com_plus(void* ptr, const void* leftPtr, const void* rightPtr) {
    complex_t* result = (complex_t*)ptr;
    const complex_t* leftCom = (complex_t*)leftPtr;
    const complex_t* rightCom = (complex_t*)rightPtr;

    *result = complex_plus(leftCom, rightCom);
}

complex_vector_t complex_vector_plus(const complex_vector_t* left, const complex_vector_t* right) {
    if (left->dimensions != right->dimensions) {
        return INVALID_COMPLEX_VEC;
    }

    size_t newDimensions = left->dimensions;
    complex_t* newValues = (complex_t*)malloc(newDimensions * sizeof(complex_t));
    check_alloc(newValues);

    linear_binary_operation(newValues, left->values, right->values, newDimensions, sizeof(complex_t), com_plus);

    complex_vector_t newVec = { newValues, newDimensions };

    return newVec;
}

vector_2d_t vec_2d_neg(const vector_2d_t* vec) {
    return vec_2d_mult(vec, -1);
}

vector_3d_t vec_3d_neg(const vector_3d_t* vec) {
    return vec_3d_mult(vec, -1);
}

vector_4d_t vec_4d_neg(const vector_4d_t* vec) {
    return vec_4d_mult(vec, -1);
}

real_vector_t real_vec_neg(const real_vector_t* vec) {
    return real_vec_mult(vec, -1);
}

complex_vector_t complex_vec_neg(const complex_vector_t* vec) {
    complex_t NEG_ONE = complex_neg(&ONE_COMPLEX);

    return complex_vec_mult(vec, &NEG_ONE);
}

vector_2d_t vec_2d_minus(const vector_2d_t* left, const vector_2d_t* right) {
    vector_2d_t negatedRight = vec_2d_neg(right);

    return vec_2d_plus(left, &negatedRight);
}

vector_3d_t vec_3d_minus(const vector_3d_t* left, const vector_3d_t* right) {
    vector_3d_t negatedRight = vec_3d_neg(right);

    return vec_3d_plus(left, &negatedRight);
}

vector_4d_t vec_4d_minus(const vector_4d_t* left, const vector_4d_t* right) {
    vector_4d_t negatedRight = vec_4d_neg(right);

    return vec_4d_plus(left, &negatedRight);
}

static void re_minus(void* ptr, const void* leftPtr, const void* rightPtr) {
    double* result = (double*)ptr;
    const double* leftReal = (double*)leftPtr;
    const double* rightReal = (double*)rightPtr;

    *result = (*leftReal) - (*rightReal);
}

real_vector_t real_vec_minus(const real_vector_t* left, const real_vector_t* right) {
    if (left->dimensions != right->dimensions) {
        return INVALID_REAL_VEC;
    }

    size_t newDimensions = left->dimensions;
    double* newValues = (double*)malloc(newDimensions * sizeof(double));
    check_alloc(newValues);

    linear_binary_operation(newValues, left->values, right->values, newDimensions, sizeof(double), re_minus);

    real_vector_t newVec = { newValues, newDimensions };

    return newVec;
}

static void com_minus(void* ptr, const void* leftPtr, const void* rightPtr) {
    complex_t* result = (complex_t*)ptr;
    const complex_t* leftCom = (complex_t*)leftPtr;
    const complex_t* rightCom = (complex_t*)rightPtr;

    *result = complex_minus(leftCom, rightCom);
}

complex_vector_t complex_vec_minus(const complex_vector_t* left, const complex_vector_t* right) {
    if (left->dimensions != right->dimensions) {
        return INVALID_COMPLEX_VEC;
    }

    size_t newDimensions = left->dimensions;
    complex_t* newValues = (complex_t*)malloc(newDimensions * sizeof(complex_t));
    check_alloc(newValues);

    linear_binary_operation(newValues, left->values, right->values, newDimensions, sizeof(complex_t), com_minus);

    complex_vector_t newVec = { newValues, newDimensions };

    return newVec;
}

vector_2d_t vec_2d_mult(const vector_2d_t* vec, const double scalar) {
    double newXAxis = vec->xAxis * scalar;
    double newYAxis = vec->yAxis * scalar;

    vector_2d_t newVec = { newXAxis, newYAxis };

    return newVec;
}

vector_3d_t vec_3d_mult(const vector_3d_t* vec, const double scalar) {
    double newXAxis = vec->xAxis * scalar;
    double newYAxis = vec->yAxis * scalar;
    double newZAxis = vec->zAxis * scalar;

    vector_3d_t newVec = { newXAxis, newYAxis, newZAxis };

    return newVec;
}

vector_4d_t vec_4d_mult(const vector_4d_t* vec, const double scalar) {
    double newWAxis = vec->wAxis * scalar;
    double newXAxis = vec->xAxis * scalar;
    double newYAxis = vec->yAxis * scalar;
    double newZAxis = vec->zAxis * scalar;

    vector_4d_t newVec = { newWAxis, newXAxis, newYAxis, newZAxis };

    return newVec;
}

real_vector_t real_vec_mult(const real_vector_t* vec, const double scalar) {
    size_t newDimensions = vec->dimensions;
    double* newValues = (double*)malloc(newDimensions * sizeof(double));
    check_alloc(newValues);

    double* ptr = vec->values;

    for (size_t index = 0; index < newDimensions; ++index) {
        newValues[index] = (*ptr) * scalar;

        ++ptr;
    }

    real_vector_t newVec = { newValues, newDimensions };

    return newVec;
}

complex_vector_t complex_vec_mult(const complex_vector_t* vec, const complex_t* scalar) {
    size_t newDimensions = vec->dimensions;
    complex_t* newValues = (complex_t*)malloc(newDimensions * sizeof(complex_t));
    check_alloc(newValues);

    complex_t* ptr = vec->values;

    for (size_t index = 0; index < newDimensions; ++index) {
        newValues[index] = complex_mult(ptr, scalar);

        ++ptr;
    }

    complex_vector_t newVec = { newValues, newDimensions };

    return newVec;
}

vector_2d_t vec_2d_div(const vector_2d_t* vec, const double scalar) {
    if (nearly_equal(0, scalar)) {
        return INVALID_VEC_2D;
    }

    return vec_2d_mult(vec, 1 / scalar);
}

vector_3d_t vec_3d_div(const vector_3d_t* vec, const double scalar) {
    if (nearly_equal(0, scalar)) {
        return INVALID_VEC_3D;
    }

    return vec_3d_mult(vec, 1 / scalar);
}

vector_4d_t vec_4d_div(const vector_4d_t* vec, const double scalar) {
    if (nearly_equal(0, scalar)) {
        return INVALID_VEC_4D;
    }

    return vec_4d_mult(vec, 1 / scalar);
}

real_vector_t real_vec_div(const real_vector_t* vec, const double scalar) {
    if (nearly_equal(0, scalar)) {
        return INVALID_REAL_VEC;
    }

    return real_vec_mult(vec, 1 / scalar);
}

complex_vector_t complex_vec_div(const complex_vector_t* vec, const complex_t* scalar) {
    if (complex_equal(&ZERO_COMPLEX, scalar)) {
        return INVALID_COMPLEX_VEC;
    }

    complex_t inverse = complex_inv(scalar);

    return complex_vec_mult(vec, &inverse);
}

double vec_2d_dot(const vector_2d_t* left, const vector_2d_t* right) {
    double xSquare = left->xAxis * right->xAxis;
    double ySquare = left->yAxis * right->yAxis;

    return xSquare + ySquare;
}

double vec_3d_dot(const vector_3d_t* left, const vector_3d_t* right) {
    double xSquare = left->xAxis * right->xAxis;
    double ySquare = left->yAxis * right->yAxis;
    double zSquare = left->zAxis * right->zAxis;

    return xSquare + ySquare + zSquare;
}

double vec_4d_dot(const vector_4d_t* left, const vector_4d_t* right) {
    double wSquare = left->wAxis * right->wAxis;
    double xSquare = left->xAxis * right->xAxis;
    double ySquare = left->yAxis * right->yAxis;
    double zSquare = left->zAxis * right->zAxis;

    return wSquare + xSquare + ySquare + zSquare;
}

double real_vec_dot(const real_vector_t* left, const real_vector_t* right) {
    if (left->dimensions != right->dimensions) {
        return NAN;
    }

    double* leftPtr = left->values;
    double* rightPtr = right->values;
    double result = 0;

    for (size_t index = 0; index < left->dimensions; ++index) {
        result += (*leftPtr) * (*rightPtr);

        ++leftPtr;
        ++rightPtr;
    }

    return result;
}

complex_t complex_vec_dot(const complex_vector_t* left, const complex_vector_t* right) {
    if (left->dimensions != right->dimensions) {
        return NAN_COMPLEX;
    }

    complex_t* leftPtr = left->values;
    complex_t* rightPtr = right->values;
    complex_t result = ZERO_COMPLEX;

    for (size_t index = 0; index < left->dimensions; ++index) {
        complex_t prod = complex_mult(leftPtr, rightPtr);
        result = complex_plus(&result, &prod);

        ++leftPtr;
        ++rightPtr;
    }

    return result;
}

vector_3d_t vec_3d_cross(const vector_3d_t* left, const vector_3d_t* right) {
    double newXAxis = left->yAxis * right->zAxis - left->zAxis * right->yAxis;
    double newYAxis = left->zAxis * right->xAxis - left->xAxis * right->zAxis;
    double newZAxis = left->xAxis * right->yAxis - left->yAxis * right->xAxis;

    vector_3d_t newVec = { newXAxis, newYAxis, newZAxis };

    return newVec;
}

vector_2d_t vec_2d_scale(const vector_2d_t* left, const vector_2d_t* right) {
    double newXAxis = left->xAxis * right->xAxis;
    double newYAxis = left->yAxis * right->yAxis;

    vector_2d_t newVec = { newXAxis, newYAxis };

    return newVec;
}

vector_3d_t vec_3d_scale(const vector_3d_t* left, const vector_3d_t* right) {
    double newXAxis = left->xAxis * right->xAxis;
    double newYAxis = left->yAxis * right->yAxis;
    double newZAxis = left->zAxis * right->zAxis;

    vector_3d_t newVec = { newXAxis, newYAxis, newZAxis };

    return newVec;
}

static void re_mult(void* ptr, const void* leftPtr, const void* rightPtr) {
    double* result = (double*)ptr;
    const double* leftReal = (double*)leftPtr;
    const double* rightReal = (double*)rightPtr;

    *result = (*leftReal) * (*rightReal);
}

real_vector_t real_vec_scale(const real_vector_t* left, const real_vector_t* right) {
    if (left->dimensions != right->dimensions) {
        return INVALID_REAL_VEC;
    }

    size_t newDimensions = left->dimensions;
    double* newValues = (double*)malloc(newDimensions * sizeof(double));
    check_alloc(newValues);

    linear_binary_operation(newValues, left->values, right->values, newDimensions, sizeof(double), re_mult);

    real_vector_t newVec = { newValues, newDimensions };

    return newVec;
}

static void com_mult(void* ptr, const void* leftPtr, const void* rightPtr) {
    complex_t* result = (complex_t*)ptr;
    const complex_t* left = (complex_t*)leftPtr;
    const complex_t* right = (complex_t*)rightPtr;

    *result = complex_mult(left, right);
}

complex_vector_t complex_vec_scale(const complex_vector_t* left, const complex_vector_t* right) {
    if (left->dimensions != right->dimensions) {
        return INVALID_COMPLEX_VEC;
    }

    size_t newDimensions = left->dimensions;
    complex_t* newValues = (complex_t*)malloc(newDimensions * sizeof(complex_t));
    check_alloc(newValues);

    linear_binary_operation(newValues, left->values, right->values, newDimensions, sizeof(complex_t), com_mult);

    complex_vector_t newVec = { newValues, newDimensions };

    return newVec;
}

double vec_2d_magn(const vector_2d_t* vec) {
    return sqrt(vec_2d_dot(vec, vec));
}

double vec_3d_magn(const vector_3d_t* vec) {
    return sqrt(vec_3d_dot(vec, vec));
}

double real_vec_magn(const real_vector_t* vec) {
    return sqrt(real_vec_dot(vec, vec));
}

complex_t complex_vec_magn(const complex_vector_t* vec) {
    complex_t selfDotProd = complex_vec_dot(vec, vec);

    return complex_sqrt(&selfDotProd);
}

vector_2d_t vec_2d_norm(const vector_2d_t* vec) {
    return vec_2d_div(vec, vec_2d_magn(vec));
}

vector_3d_t vec_3d_norm(const vector_3d_t* vec) {
    return vec_3d_div(vec, vec_3d_magn(vec));
}

real_vector_t real_vec_norm(const real_vector_t* vec) {
    return real_vec_div(vec, real_vec_magn(vec));
}

complex_vector_t complex_vec_norm(const complex_vector_t* vec) {
    complex_t magnitude = complex_vec_magn(vec);

    return complex_vec_div(vec, &magnitude);
}

double vec_2d_angle(const vector_2d_t* vec1, const vector_2d_t* vec2) {
    double dotProd = vec_2d_dot(vec1, vec2);
    double magnProd = vec_2d_magn(vec1) * vec_2d_magn(vec2);

    return acos(dotProd / magnProd);
}

double vec_3d_angle(const vector_3d_t* vec1, const vector_3d_t* vec2) {
    double dotProd = vec_3d_dot(vec1, vec2);
    double magnProd = vec_3d_magn(vec1) * vec_3d_magn(vec2);

    return acos(dotProd / magnProd);
}

double real_vec_angle(const real_vector_t* vec1, const real_vector_t* vec2) {
    double dotProd = real_vec_dot(vec1, vec2);
    double magnProd = real_vec_magn(vec1) * real_vec_magn(vec2);

    return acos(dotProd / magnProd);
}

complex_t complex_vec_angle(const complex_vector_t* vec1, const complex_vector_t* vec2) {
    complex_t magn1 = complex_vec_magn(vec1);
    complex_t magn2 = complex_vec_magn(vec2);
    complex_t magnProd = complex_mult(&magn1, &magn2);
    complex_t dotProd = complex_vec_dot(vec1, vec2);
    complex_t quotient = complex_div(&dotProd, &magnProd);

    return complex_acos(&quotient);
}
