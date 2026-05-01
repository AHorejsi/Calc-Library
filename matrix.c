#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "floating_points.h"
#include "alloc.h"
#include "matrix.h"

#define DIMENSIONS_FOR_COMPLEX 2
#define DIMENSIONS_FOR_QUATERNION 4


typedef struct {
    void* mat;
    size_t* ignore;
    size_t amountIgnored;
} sub_matrix_t;

typedef char byte_t;
typedef void(*unary_operation_t)(void*, const void*);
typedef void(*binary_operation_t)(void*, const void*, const void*);
typedef void(*determinant_t)(void*, const sub_matrix_t*);

const real_matrix_t INVALID_REAL_MATRIX = { NULL, 0, 0 };
const complex_matrix_t INVALID_COMPLEX_MATRIX = { NULL, 0, 0 };

void destroy_real_matrix(real_matrix_t* mat) {
    free(mat->values);
}

void destroy_complex_matrix(complex_matrix_t* mat) {
    free(mat->values);
}

static size_t actual_index(const size_t cols, const size_t rowIndex, const size_t colIndex) {
    return rowIndex * cols + colIndex;
}

double real_matrix_row_get(const real_matrix_t* mat, const size_t rowIndex, const size_t colIndex) {
    return mat->values[actual_index(mat->cols, rowIndex, colIndex)];
}

complex_t complex_matrix_row_get(const complex_matrix_t* mat, const size_t rowIndex, const size_t colIndex) {
    return mat->values[actual_index(mat->cols, rowIndex, colIndex)];
}

double real_matrix_col_get(const real_matrix_t* mat, const size_t colIndex, const size_t rowIndex) {
    return real_matrix_row_get(mat, rowIndex, colIndex);
}

complex_t complex_matrix_col_get(const complex_matrix_t* mat, const size_t rowIndex, const size_t colIndex) {
    return complex_matrix_col_get(mat, rowIndex, colIndex);
}

static void set_real_value(double* values, const size_t cols, const size_t rowIndex, const size_t colIndex, const double newValue) {
    size_t actualIndex = actual_index(cols, rowIndex, colIndex);

    values[actualIndex] = newValue;
}

void real_matrix_row_set(real_matrix_t* mat, const size_t rowIndex, const size_t colIndex, const double newValue) {
    set_real_value(mat->values, mat->cols, rowIndex, colIndex, newValue);
}

void real_matrix_col_set(real_matrix_t* mat, const size_t colIndex, const size_t rowIndex, const double newValue) {
    return real_matrix_row_set(mat, rowIndex, colIndex, newValue);
}

static void set_complex_value(complex_t* values, const size_t cols, const size_t rowIndex, const size_t colIndex, const complex_t* newValue) {
    size_t actualIndex = actual_index(cols, rowIndex, colIndex);

    values[actualIndex] = *newValue;
}

void complex_matrix_row_set(complex_matrix_t* mat, const size_t rowIndex, const size_t colIndex, const complex_t newValue) {
    set_complex_value(mat->values, mat->cols, rowIndex, colIndex, &newValue);
}

void complex_matrix_col_set(complex_matrix_t* mat, const size_t colIndex, const size_t rowIndex, const complex_t newValue) {
    complex_matrix_row_set(mat, rowIndex, colIndex, newValue);
}

static bool is_square_helper(const size_t rows, const size_t cols) {
    return rows == cols;
}

bool is_real_square(const real_matrix_t* mat) {
    return is_square_helper(mat->rows, mat->cols);
}

bool is_complex_square(const complex_matrix_t* mat) {
    return is_square_helper(mat->rows, mat->cols);
}

static bool are_addable_helper(const size_t leftRows, const size_t leftCols, const size_t rightRows, const size_t rightCols) {
    return leftRows == rightRows && leftCols == rightCols;
}

bool are_real_addable(const real_matrix_t* left, const real_matrix_t* right) {
    return are_addable_helper(left->rows, left->cols, right->rows, right->cols);
}

bool are_complex_addable(const complex_matrix_t* left, const complex_matrix_t* right) {
    return are_addable_helper(left->rows, left->cols, right->rows, right->cols);
}

static bool are_multipliable_helper(const size_t leftCols, const size_t rightRows) {
    return leftCols == rightRows;
}

bool are_real_multipliable(const real_matrix_t* left, const real_matrix_t* right) {
    return are_multipliable_helper(left->cols, right->rows);
}

bool are_complex_multipliable(const complex_matrix_t* left, const complex_matrix_t* right) {
    return are_multipliable_helper(left->cols, right->rows);
}

real_matrix_t from_complex_to_real_matrix(const complex_t* com) {
    double* values = (double*)malloc(DIMENSIONS_FOR_COMPLEX * DIMENSIONS_FOR_COMPLEX * sizeof(double));
    check_alloc(values);

    values[0] = com->real;
    values[1] = -com->imag;
    values[2] = com->imag;
    values[3] = com->real;

    real_matrix_t new = { values, DIMENSIONS_FOR_COMPLEX, DIMENSIONS_FOR_COMPLEX };

    return new;
}

real_matrix_t from_quaternion_to_real_matrix(const quaternion_t* quat) {
    double* values = (double*)malloc(DIMENSIONS_FOR_QUATERNION * DIMENSIONS_FOR_QUATERNION * sizeof(double));
    check_alloc(values);

    values[0] = quat->real;
    values[1] = -quat->imag0;
    values[2] = -quat->imag1;
    values[3] = -quat->imag2;
    values[4] = quat->imag0;
    values[5] = quat->real;
    values[6] = -quat->imag2;
    values[7] = quat->imag1;
    values[8] = quat->imag1;
    values[9] = quat->imag2;
    values[10] = quat->real;
    values[11] = -quat->imag0;
    values[12] = quat->imag2;
    values[13] = -quat->imag1;
    values[14] = quat->imag0;
    values[15] = quat->real;

    real_matrix_t new = { values, DIMENSIONS_FOR_QUATERNION, DIMENSIONS_FOR_QUATERNION };

    return new;
}

complex_matrix_t from_quaternion_to_complex_matrix(const quaternion_t* quat) {
    complex_t* values = (complex_t*)malloc(DIMENSIONS_FOR_QUATERNION * sizeof(complex_t));
    check_alloc(values);

    complex_t topLeft = { quat->real, quat->imag0 };
    complex_t topRight = { quat->imag1, quat->imag2 };
    complex_t bottomLeft = { -(quat->imag1), quat->imag2 };
    complex_t bottomRight = { quat->real, -(quat->imag0) };

    values[0] = topLeft;
    values[1] = topRight;
    values[2] = bottomLeft;
    values[3] = bottomRight;

    complex_matrix_t new = { values, DIMENSIONS_FOR_COMPLEX, DIMENSIONS_FOR_COMPLEX };

    return new;
}

static size_t actual_length_helper(const size_t rows, const size_t cols) {
    return rows * cols;
}

size_t actual_length_real(const real_matrix_t* mat) {
    return actual_length_helper(mat->rows, mat->cols);
}

size_t actual_length_complex(const complex_matrix_t* mat) {
    return actual_length_helper(mat->rows, mat->cols);
}

static size_t actual_row(const size_t index, const size_t cols) {
    return index / cols;
}

static size_t actual_col(const size_t index, const size_t cols) {
    return index % cols;
}

real_matrix_t init_real_matrix(const size_t rows, const size_t cols) {
    size_t actualLength = actual_length_helper(rows, cols);

    double* values = (double*)malloc(actualLength * sizeof(double));
    check_alloc(values);

    real_matrix_t new = { values, rows, cols };

    return new;
}

complex_matrix_t init_complex_matrix(const size_t rows, const size_t cols) {
    size_t actualLength = actual_length_helper(rows, cols);

    complex_t* values = (complex_t*)malloc(actualLength * sizeof(complex_t));
    check_alloc(values);

    complex_matrix_t new = { values, rows, cols };

    return new;
}

real_matrix_t zero_real_matrix(const size_t rows, const size_t cols) {
    return fill_real_matrix(rows, cols, 0);
}

complex_matrix_t zero_complex_matrix(const size_t rows, const size_t cols) {
    return fill_complex_matrix(rows, cols, ZERO_COMPLEX);
}

real_matrix_t identity_real_matrix(const size_t dimensions) {
    real_matrix_t new = init_real_matrix(dimensions, dimensions);
    
    double* ptr = new.values;
    size_t actualLength = actual_length_helper(dimensions, dimensions);

    size_t rowIndex = 0;
    size_t colIndex = 0;

    for (size_t index = 0; index < actualLength; ++index) {
        *ptr = (rowIndex == colIndex) ? 1 : 0;
        ++ptr;

        ++colIndex;
        if (colIndex == dimensions) {
            ++rowIndex;
            colIndex = 0;
        }
    }

    return new;
}

complex_matrix_t identity_complex_matrix(const size_t dimensions) {
    complex_matrix_t new = init_complex_matrix(dimensions, dimensions);

    complex_t* ptr = new.values;
    size_t actualLength = actual_length_helper(dimensions, dimensions);

    size_t rowIndex = 0;
    size_t colIndex = 0;

    for (size_t index = 0; index < actualLength; ++index) {
        *ptr = (rowIndex == colIndex) ? ONE_COMPLEX : ZERO_COMPLEX;
        ++ptr;

        ++colIndex;
        if (colIndex == dimensions) {
            ++rowIndex;
            colIndex = 0;
        }
    }

    return new;
}

real_matrix_t fill_real_matrix(const size_t rows, const size_t cols, const double fill) {
    real_matrix_t new = init_real_matrix(rows, cols);
    size_t actualLength = actual_length_real(&new);

    double* ptr = new.values;

    for (size_t index = 0; index < actualLength; ++index) {
        *ptr = fill;
        ++ptr;
    }

    return new;
}

complex_matrix_t fill_complex_matrix(const size_t rows, const size_t cols, const complex_t fill) {
    complex_matrix_t new = init_complex_matrix(rows, cols);
    size_t actualLength = actual_length_complex(&new);

    complex_t* ptr = new.values;

    for (size_t index = 0; index < actualLength; ++index) {
        *ptr = fill;
        ++ptr;
    }

    return new;
}

static double* copy_real_vec(const real_vector_t* vec) {
    size_t length = vec->dimensions;

    double* values = (double*)malloc(length * sizeof(double));
    check_alloc(values);

    double* vecPtr = vec->values;
    double* matPtr = values;

    for (size_t index = 0; index < length; ++index) {
        *matPtr = *vecPtr;
        
        ++vecPtr;
        ++matPtr;
    }

    return values;
}

real_matrix_t real_row_matrix(const real_vector_t* vec) {
    double* values = copy_real_vec(vec);
    real_matrix_t new = { values, 1, vec->dimensions };

    return new;
}

real_matrix_t real_col_matrix(const real_vector_t* vec) {
    double* values = copy_real_vec(vec);
    real_matrix_t new = { values, vec->dimensions, 1 };

    return new;
}

static complex_t* copy_complex_vec(const complex_vector_t* vec) {
    size_t length = vec->dimensions;

    complex_t* values = (complex_t*)malloc(length * sizeof(complex_t));
    check_alloc(values);

    complex_t* vecPtr = vec->values;
    complex_t* matPtr = values;

    for (size_t index = 0; index < length; ++index) {
        *matPtr = *vecPtr;

        ++vecPtr;
        ++matPtr;
    }

    return values;
}

complex_matrix_t complex_row_matrix(const complex_vector_t* vec) {
    complex_t* values = copy_complex_vec(vec);
    complex_matrix_t new = { values, 1, vec->dimensions };

    return new;
}

complex_matrix_t complex_col_matrix(const complex_vector_t* vec) {
    complex_t* values = copy_complex_vec(vec);
    complex_matrix_t new = { values, vec->dimensions, 1 };

    return new;
}

static void* linear_binary_operation(void* left, void* right, const size_t length, const size_t byteSize, const binary_operation_t op) {
    byte_t* values = (byte_t*)malloc(length * byteSize);
    check_alloc(values);

    byte_t* ptr = values;
    byte_t* leftPtr = (byte_t*)left;
    byte_t* rightPtr = (byte_t*)right;

    for (size_t index = 0; index < length; ++index) {
        op(ptr, leftPtr, rightPtr);

        ptr += byteSize;
        leftPtr += byteSize;
        rightPtr += byteSize;
    }

    return values;
}

static void re_plus(void* ptr, const void* left, const void* right) {
    double* destPtr = (double*)ptr;
    const double* leftPtr = (double*)left;
    const double* rightPtr = (double*)right;

    *destPtr = (*leftPtr) + (*rightPtr);
}

real_matrix_t real_matrix_plus(const real_matrix_t* left, const real_matrix_t* right) {
    if (!are_real_addable(left, right)) {
        return INVALID_REAL_MATRIX;
    }

    size_t length = actual_length_real(left);
    double* values = (double*)linear_binary_operation(left->values, right->values, length, sizeof(double), re_plus);

    real_matrix_t new = { values, left->rows, left->cols };

    return new;
}

static void com_plus(void* ptr, const void* left, const void* right) {
    complex_t* destPtr = (complex_t*)ptr;
    const complex_t* leftPtr = (complex_t*)left;
    const complex_t* rightPtr = (complex_t*)right;

    *destPtr = complex_plus(leftPtr, rightPtr);
}

complex_matrix_t complex_matrix_plus(const complex_matrix_t* left, const complex_matrix_t* right) {
    if (!are_complex_addable(left, right)) {
        return INVALID_COMPLEX_MATRIX;
    }

    size_t length = actual_length_complex(left);
    complex_t* values = (complex_t*)linear_binary_operation(left->values, right->values, length, sizeof(complex_t), com_plus);

    complex_matrix_t new = { values, left->rows, left->cols };

    return new;
}

static void* linear_unary_operation(void* values, const size_t length, const size_t byteSize, const unary_operation_t op) {
    byte_t* newValues = (byte_t*)malloc(length * byteSize);
    check_alloc(values);

    byte_t* newPtr = newValues;
    byte_t* oldPtr = (byte_t*)values;

    for (size_t index = 0; index < length; ++index) {
        op(newPtr, oldPtr);

        oldPtr += byteSize;
        newPtr += byteSize;
    }

    return newValues;
}

static void re_neg(void* ptr, const void* value) {
    double* destPtr = (double*)ptr;
    const double* srcPtr = (const double*)value;

    *destPtr = -(*srcPtr);
}

real_matrix_t real_matrix_negate(const real_matrix_t* mat) {
    size_t length = actual_length_real(mat);
    double* values = (double*)linear_unary_operation(mat->values, length, sizeof(double), re_neg);

    real_matrix_t new = { values, mat->rows, mat->cols };

    return new;
}

static void com_neg(void* ptr, const void* value) {
    complex_t* destPtr = (complex_t*)ptr;
    const complex_t* srcPtr = (const complex_t*)value;

    *destPtr = complex_neg(srcPtr);
}

complex_matrix_t complex_matrix_negate(const complex_matrix_t* mat) {
    size_t length = actual_length_complex(mat);
    complex_t* values = (complex_t*)linear_unary_operation(mat->values, length, sizeof(complex_t), com_neg);

    complex_matrix_t new = { values, mat->rows, mat->cols };

    return new;
}

static void re_minus(void* ptr, const void* left, const void* right) {
    double* destPtr = (double*)ptr;
    const double* leftPtr = (double*)left;
    const double* rightPtr = (double*)right;

    *destPtr = (*leftPtr) - (*rightPtr);
}

real_matrix_t real_matrix_minus(const real_matrix_t* left, const real_matrix_t* right) {
    if (!are_real_addable(left, right)) {
        return INVALID_REAL_MATRIX;
    }

    size_t length = actual_length_real(left);
    double* values = (double*)linear_binary_operation(left->values, right->values, length, sizeof(complex_t), re_minus);

    real_matrix_t new = { values, left->rows, left->cols };

    return new;
}

static void com_minus(void* ptr, const void* left, const void* right) {
    complex_t* destPtr = (complex_t*)ptr;
    const complex_t* leftPtr = (complex_t*)left;
    const complex_t* rightPtr = (complex_t*)right;

    *destPtr = complex_minus(leftPtr, rightPtr);
}

complex_matrix_t complex_matrix_minus(const complex_matrix_t* left, const complex_matrix_t* right) {
    if (!are_complex_addable(left, right)) {
        return INVALID_COMPLEX_MATRIX;
    }

    size_t length = actual_length_complex(left);
    complex_t* values = (complex_t*)linear_binary_operation(left->values, right->values, length, sizeof(complex_t), com_minus);

    complex_matrix_t new = { values, left->rows, left->cols };

    return new;
}

static void dot_prod(void* ptr, void* left, void* right, const size_t byteSize, const size_t rowIndex, const size_t colIndex, const size_t leftCols, const size_t rightRows, const binary_operation_t op) {
    byte_t* resultPtr = (byte_t*)ptr;
    byte_t* leftPtr = (byte_t*)left;
    byte_t* rightPtr = (byte_t*)right;

    for (size_t index = 0; index < rightRows; ++index) {
        op(resultPtr, leftPtr, rightPtr);

        resultPtr += byteSize;
        leftPtr += byteSize;
        rightPtr += byteSize * leftCols;
    }
}

static void re_product(void* result, const void* left, const void* right) {
    double* resultPtr = (double*)result;
    const double* leftPtr = (const double*)left;
    const double* rightPtr = (const double*)right;

    *resultPtr += (*leftPtr) * (*rightPtr);
}

real_matrix_t real_matrix_mult(const real_matrix_t* left, const real_matrix_t* right) {
    if (!are_real_multipliable(left, right)) {
        return INVALID_REAL_MATRIX;
    }

    size_t leftRows = left->rows;
    size_t rightCols = right->cols;
    size_t actualLength = leftRows * rightCols;

    double* values = (double*)calloc(actualLength, sizeof(double));
    check_alloc(values);

    size_t index = 0;
    double* ptr = values;

    for (size_t rowIndex = 0; rowIndex < leftRows; ++rowIndex) {
        for (size_t colIndex = 0; colIndex < rightCols; ++colIndex) {
            dot_prod(
                ptr,
                left->values,
                right->values,
                sizeof(double),
                rowIndex,
                colIndex,
                left->cols,
                right->rows,
                re_product
            );

            ++ptr;
        }
    }

    real_matrix_t new = { values, leftRows, rightCols };

    return new;
}

static void com_product(void* result, const void* left, const void* right) {
    complex_t* resultPtr = (complex_t*)result;
    const complex_t* leftPtr = (complex_t*)left;
    const complex_t* rightPtr = (complex_t*)right;

    complex_t prod = complex_mult(leftPtr, rightPtr);
    *resultPtr = complex_plus(resultPtr, &prod);
}

complex_matrix_t complex_matrix_mult(const complex_matrix_t* left, const complex_matrix_t* right) {
    if (!are_complex_multipliable(left, right)) {
        return INVALID_COMPLEX_MATRIX;
    }

    size_t leftRows = left->rows;
    size_t rightCols = right->cols;
    size_t actualLength = leftRows * rightCols;

    complex_t* values = (complex_t*)calloc(actualLength, sizeof(complex_t));
    check_alloc(values);

    size_t index = 0;
    complex_t* ptr = values;

    for (size_t rowIndex = 0; leftRows; ++rowIndex) {
        for (size_t colIndex = 0; colIndex < rightCols; ++colIndex) {
            dot_prod(
                ptr,
                left->values,
                right->values,
                sizeof(complex_t),
                rowIndex,
                colIndex,
                left->cols,
                right->rows,
                com_product
            );

            ++ptr;
        }
    }

    complex_matrix_t new = { values, leftRows, rightCols };

    return new;
}

static void scalar_mult(void* newValues, void* oldValues, const size_t byteSize, const size_t length, const void* scalar, const binary_operation_t op) {
    byte_t* oldPtr = (byte_t*)oldValues;
    byte_t* newPtr = (byte_t*)newValues;

    for (size_t index = 0; index < length; ++index) {
        op(newPtr, oldPtr, scalar);

        oldPtr += byteSize;
        newPtr += byteSize;
    }
}

static void re_mult(void* ptr, const void* left, const void* right) {
    double* destPtr = (double*)ptr;
    const double* leftPtr = (const double*)left;
    const double* rightPtr = (const double*)right;

    *destPtr = (*leftPtr) * (*rightPtr);
}

real_matrix_t real_matrix_scalar_mult(const real_matrix_t* mat, const double scalar) {
    size_t actualLength = actual_length_real(mat);

    double* values = (double*)malloc(actualLength * sizeof(double));
    check_alloc(values);

    scalar_mult(values, mat->values, sizeof(double), actualLength, &scalar, re_mult);

    real_matrix_t new = { values, mat->rows, mat->cols };

    return new;
}

static void com_mult(void* ptr, const void* left, const void* right) {
    complex_t* destPtr = (complex_t*)ptr;
    complex_t* leftPtr = (complex_t*)left;
    complex_t* rightPtr = (complex_t*)right;

    *destPtr = complex_mult(leftPtr, rightPtr);
}

complex_matrix_t complex_matrix_scalar_mult(const complex_matrix_t* mat, const complex_t* scalar) {
    size_t actualLength = actual_length_complex(mat);

    complex_t* values = (complex_t*)malloc(actualLength * sizeof(complex_t));
    check_alloc(values);

    scalar_mult(values, mat->values, sizeof(complex_t), actualLength, scalar, com_mult);

    complex_matrix_t new = { values, mat->rows, mat->cols };

    return new;
}

real_matrix_t real_matrix_scale(const real_matrix_t* left, const real_matrix_t* right) {
    if (!are_real_addable(left, right)) {
        return INVALID_REAL_MATRIX;
    }

    size_t length = actual_length_real(left);
    double* values = (double*)linear_binary_operation(left->values, right->values, length, sizeof(double), re_mult);

    real_matrix_t new = { values, left->rows, left->cols };

    return new;
}

complex_matrix_t complex_matrix_scale(const complex_matrix_t* left, const complex_matrix_t* right) {
    if (!are_complex_addable(left, right)) {
        return INVALID_COMPLEX_MATRIX;
    }

    size_t length = actual_length_complex(left);
    complex_t* values = (complex_t*)linear_binary_operation(left->values, right->values, length, sizeof(complex_t), com_mult);

    complex_matrix_t new = { values, left->rows, right->cols };

    return new;
}

real_matrix_t real_matrix_mult_real_vec(const real_matrix_t* left, const real_vector_t* right) {
    real_matrix_t asMatrix = real_col_matrix(right);
    real_matrix_t result = real_matrix_mult(left, &asMatrix);

    destroy_real_matrix(&asMatrix);

    return result;
}

complex_matrix_t complex_matrix_mult_complex_vec(const complex_matrix_t* left, const complex_vector_t* right) {
    complex_matrix_t asMatrix = complex_col_matrix(right);
    complex_matrix_t result = complex_matrix_mult(left, &asMatrix);

    destroy_complex_matrix(&asMatrix);

    return result;
}

real_matrix_t real_vec_mult_real_matrix(const real_vector_t* left, const real_matrix_t* right) {
    real_matrix_t asMatrix = real_row_matrix(left);
    real_matrix_t result = real_matrix_mult(&asMatrix, right);

    destroy_real_matrix(&asMatrix);

    return result;
}

complex_matrix_t complex_vec_mult_complex_matrix(const complex_vector_t* left, const complex_matrix_t* right) {
    complex_matrix_t asMatrix = complex_row_matrix(left);
    complex_matrix_t result = complex_matrix_mult(&asMatrix, right);

    destroy_complex_matrix(&asMatrix);

    return result;
}

static double real_det_helper(const real_matrix_t* mat) {
    if (2 == mat->rows) {
        return mat->values[0] * mat->values[3] - mat->values[1] * mat->values[2]; 
    }

    size_t length = mat->rows;

    uint8_t coef = 1;
    double result = 0;

    for (size_t index = 0; index < length; ++index) {
        double current = real_matrix_row_get(mat, 0, index);
        real_matrix_t sub = sub_real_matrix(mat, 0, index);

        result += coef * current * real_det_helper(&sub);

        coef = -coef;
    }

    return result;
}

double real_matrix_determinant(const real_matrix_t* mat) {
    if (!is_real_square(mat)) {
        return NAN;
    }
    else {
        return (1 == mat->rows) ? mat->values[0] : real_det_helper(mat);
    }
}

static void matrix_of_minors(const real_matrix_t* mat, double* values) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    size_t index = 0;

    for (size_t rowIndex = 0; rowIndex < rows; ++rowIndex) {
        for (size_t colIndex = 0; colIndex < cols; ++colIndex) {
            real_matrix_t sub = sub_real_matrix(mat, rowIndex, colIndex);

            values[index] = real_matrix_determinant(&sub);

            destroy_real_matrix(&sub);
        }
    }
}

static void matrix_of_cofactors(const real_matrix_t* mat, double* values) {
    bool negative = false;

    size_t length = mat->rows * mat->cols;

    for (size_t index = 0; index < length; ++index) {
        if (negative) {
            values[index] = -(values[index]);
        }

        negative = !negative;
    }
}

static void swap_reals(double* ptr1, double* ptr2) {
    double temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

static void matrix_adjugate(double* values, const size_t rowLength) {
    for (size_t rowIndex = 1; rowIndex < rowLength; ++rowIndex) {
        size_t colLength = rowIndex;

        for (size_t colIndex = 0; colIndex < colLength; ++colIndex) {
            size_t index1 = actual_index(rowLength, rowIndex, colIndex);
            size_t index2 = actual_index(rowLength, colIndex, rowIndex);

            double* ptr1 = values + index1;
            double* ptr2 = values + index2;

            swap_reals(ptr1, ptr2);
        }
    }
}

real_matrix_t real_matrix_inv(const real_matrix_t* mat) {
    double det = real_matrix_determinant(mat);

    if (0 == det) {
        return INVALID_REAL_MATRIX;
    }

    size_t length = actual_length_real(mat);

    double* values = (double*)malloc(length * sizeof(double));
    check_alloc(values);

    matrix_of_minors(mat, values);
    matrix_of_cofactors(mat, values);
    matrix_adjugate(values, mat->rows);

    double invDet = 1 / det;
    scalar_mult(values, mat->values, sizeof(double), length, &invDet, re_mult);

    real_matrix_t new = { values, mat->rows, mat->cols };

    return new;
}

real_matrix_t real_matrix_div(const real_matrix_t* left, const real_matrix_t* right) {
    if (!(are_real_multipliable(left, right) && is_real_square(right))) {
        return INVALID_REAL_MATRIX;
    }

    real_matrix_t rightInverse = real_matrix_inv(right);
    real_matrix_t result = real_matrix_mult(left, &rightInverse);

    destroy_real_matrix(&rightInverse);

    return result;
}

real_matrix_t real_matrix_scalar_div(const real_matrix_t* mat, const double scalar) {
    if (nearly_equal(0, scalar)) {
        return INVALID_REAL_MATRIX;
    }

    return real_matrix_scalar_mult(mat, 1 / scalar);
}

complex_matrix_t complex_matrix_scalar_div(const complex_matrix_t* mat, const complex_t* scalar) {
    if (complex_equal(&ZERO_COMPLEX, scalar)) {
        return INVALID_COMPLEX_MATRIX;
    }

    complex_t inverse = complex_inv(scalar);

    return complex_matrix_scalar_mult(mat, &inverse);
}

static void do_swap(void* ptr1, void* ptr2, void* temp, const size_t byteSize) {
    memcpy(temp, ptr1, byteSize);
    memcpy(ptr1, ptr2, byteSize);
    memcpy(ptr2, temp, byteSize);
}

static void* do_transpose(
    void* oldValues,
    const size_t rows,
    const size_t cols,
    const size_t byteSize
) {
    size_t length = rows * cols;
    void* newValues = malloc(length * byteSize);

    byte_t* oldPtr = (byte_t*)oldValues;
    byte_t* newPtr = (byte_t*)newValues;
    void* tempPtr = malloc(byteSize);

    for (size_t index = 0; index < length; ++index) {
        size_t rowIndex = actual_row(index, rows);
        size_t colIndex = actual_col(index, cols);

        do_swap(oldPtr, newPtr, tempPtr, byteSize);

        oldPtr += byteSize;
        newPtr += byteSize;
    }

    free(tempPtr);

    return newValues;
}

real_matrix_t real_matrix_transpose(const real_matrix_t* mat) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    double* values = (double*)do_transpose(mat->values, rows, cols, sizeof(double));
    real_matrix_t new = { values, cols, rows };

    return new;
}

complex_matrix_t complex_matrix_transpose(const complex_matrix_t* mat) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    complex_t* values = (complex_t*)do_transpose(mat->values, rows, cols, sizeof(complex_t));
    complex_matrix_t new = { values, cols, rows };

    return new;
}

real_matrix_t sub_real_matrix(const real_matrix_t* mat, const size_t ignoreRowIndex, const size_t ignoreColIndex) {
    size_t actualLength = actual_length_real(mat);

    size_t rows = mat->rows - 1;
    size_t cols = mat->cols - 1;

    double* values = (double*)malloc(rows * cols * sizeof(double));
    check_alloc(values);

    size_t oldIndex = 0;
    size_t newIndex = 0;

    while (oldIndex < actualLength) {
        size_t currentRowIndex = actual_row(oldIndex, cols);
        if (ignoreRowIndex == currentRowIndex) {
            oldIndex += cols;

            continue;
        }

        size_t currentColIndex = actual_col(oldIndex, cols);
        if (ignoreColIndex != currentColIndex) {
            values[newIndex] = mat->values[oldIndex];
            
            ++newIndex;
        }

        ++oldIndex;
    }

    real_matrix_t new = { values, rows - 1, cols - 1 };

    return new;
}

static void* copy_values(const void* oldValues, const size_t actualLength, const size_t byteSize) {
    double* newValues = (double*)malloc(actualLength * byteSize);
    check_alloc(newValues);

    byte_t* oldPtr = (byte_t*)oldValues;
    byte_t* newPtr = (byte_t*)newValues;

    for (size_t index = 0; index < actualLength; ++index) {
        memcpy(newPtr, oldPtr, byteSize);

        oldPtr += byteSize;
        newPtr += byteSize;
    }

    return newValues;
}

static size_t row_start_index(const size_t rowIndex, const size_t cols) {
    return rowIndex * cols;
}

real_matrix_t row_add_real(const real_matrix_t* mat, const size_t sourceRowIndex, const size_t targetRowIndex) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    double* values = (double*)copy_values(mat->values, rows * cols, sizeof(double));

    double* source = mat->values + row_start_index(sourceRowIndex, cols);
    double* target = values + row_start_index(targetRowIndex, cols);

    for (size_t colIndex = 0; colIndex < cols; ++colIndex) {
        *target += *source;

        ++source;
        ++target;
    }

    real_matrix_t new = { values, rows, cols };

    return new;
}

complex_matrix_t row_add_complex(const complex_matrix_t* mat, const size_t sourceRowIndex, const size_t targetRowIndex) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    complex_t* values = (complex_t*)copy_values(mat->values, rows * cols, sizeof(complex_t));

    complex_t* source = mat->values + row_start_index(sourceRowIndex, cols);
    complex_t* target = values + row_start_index(targetRowIndex, cols);

    for (size_t colIndex = 0; colIndex < cols; ++colIndex) {
        *target = complex_plus(target, source);

        ++source;
        ++target;
    }

    complex_matrix_t new = { values, rows, cols };

    return new;
}

real_matrix_t row_mult_real(const real_matrix_t* mat, const double scalar, const size_t targetRowIndex) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    double* values = (double*)copy_values(mat->values, rows * cols, sizeof(double));

    double* target = values + row_start_index(targetRowIndex, cols);

    for (size_t colIndex = 0; colIndex < cols; ++colIndex) {
        *target *= scalar;

        ++target;
    }

    real_matrix_t new = { values, rows, cols };

    return new;
}

complex_matrix_t row_mult_complex(const complex_matrix_t* mat, const complex_t* scalar, const size_t targetRowIndex) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    complex_t* values = (complex_t*)copy_values(mat->values, rows * cols, sizeof(complex_t));

    complex_t* target = values + row_start_index(targetRowIndex, cols);

    for (size_t colIndex = 0; colIndex < cols; ++colIndex) {
        *target = complex_mult(target, scalar);

        ++target;
    }

    complex_matrix_t new = { values, rows, cols };

    return new;
}

real_matrix_t row_swap_real(const real_matrix_t* mat, const size_t rowIndex1, const size_t rowIndex2) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    double* values = (double*)copy_values(mat->values, rows * cols, sizeof(double));

    double* ptr1 = mat->values + row_start_index(rowIndex1, cols);
    double* ptr2 = values + row_start_index(rowIndex2, cols);

    for (size_t colIndex = 0; colIndex < cols; ++colIndex) {
        swap_reals(ptr1, ptr2);

        ++ptr1;
        ++ptr2;
    }

    real_matrix_t new = { values, rows, cols };

    return new;
}

static void swap_complex(complex_t* value1, complex_t* value2) {
    complex_t temp = *value1;
    *value1 = *value2;
    *value2 = temp;
}

complex_matrix_t row_swap_complex(const complex_matrix_t* mat, const size_t rowIndex1, const size_t rowIndex2) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    complex_t* values = (complex_t*)copy_values(mat->values, rows * cols, sizeof(complex_t));

    complex_t* ptr1 = mat->values + row_start_index(rowIndex1, cols);
    complex_t* ptr2 = values + row_start_index(rowIndex2, cols);

    for (size_t colIndex = 0; colIndex < cols; ++colIndex) {
        swap_complex(ptr1, ptr2);

        ++ptr1;
        ++ptr2;
    }

    complex_matrix_t new = { values, rows, cols };

    return new;
}

real_matrix_t col_add_real(const real_matrix_t* mat, const size_t sourceColIndex, const size_t targetColIndex) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    double* values = (double*)copy_values(mat->values, rows * cols, sizeof(double));

    double* source = mat->values + sourceColIndex;
    double* target = values + targetColIndex;

    for (size_t rowIndex = 0; rowIndex < rows; ++rowIndex) {
        *target += *source;

        source += cols;
        target += cols;
    }

    real_matrix_t new = { values, rows, cols };

    return new;
}

complex_matrix_t col_add_complex(const complex_matrix_t* mat, const size_t sourceColIndex, const size_t targetColIndex) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    complex_t* values = (complex_t*)copy_values(mat->values, rows * cols, sizeof(complex_t));

    complex_t* source = mat->values + sourceColIndex;
    complex_t* target = values + targetColIndex;

    for (size_t rowIndex = 0; rowIndex < rows; ++rowIndex) {
        *target = complex_plus(target, source);

        source += cols;
        target += cols;
    }

    complex_matrix_t new = { values, rows, cols };

    return new;
}

real_matrix_t col_mult_real(const real_matrix_t* mat, const double scalar, const size_t targetColIndex) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    double* values = (double*)copy_values(mat->values, rows * cols, sizeof(double));

    double* target = values + targetColIndex;

    for (size_t rowIndex = 0; rowIndex < rows; ++rowIndex) {
        *target *= scalar;

        target += cols;
    }

    real_matrix_t new = { values, rows, cols };

    return new;
}

complex_matrix_t col_mult_complex(const complex_matrix_t* mat, const complex_t* scalar, const size_t targetColIndex) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    complex_t* values = (complex_t*)copy_values(mat->values, rows * cols, sizeof(complex_t));

    complex_t* target = values + targetColIndex;

    for (size_t rowIndex = 0; rowIndex < rows; ++rowIndex) {
        *target = complex_mult(target, scalar);

        target += cols;
    }

    complex_matrix_t new = { values, rows, cols };

    return new;
}

real_matrix_t col_swap_real(const real_matrix_t* mat, const size_t colIndex1, const size_t colIndex2) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    double* values = copy_values(mat->values, rows * cols, sizeof(double));

    double* ptr1 = values + colIndex1;
    double* ptr2 = values + colIndex2;

    for (size_t rowIndex = 0; rowIndex < rows; ++rowIndex) {
        swap_reals(ptr1, ptr2);

        ptr1 += cols;
        ptr2 += cols;
    }

    real_matrix_t new = { values, rows, cols };

    return new;
}

complex_matrix_t col_swap_complex(const complex_matrix_t* mat, const size_t colIndex1, const size_t colIndex2) {
    size_t rows = mat->rows;
    size_t cols = mat->cols;

    complex_t* values = (complex_t*)copy_values(mat->values, rows * cols, sizeof(complex_t));

    complex_t* ptr1 = values + colIndex1;
    complex_t* ptr2 = values + colIndex2;

    for (size_t rowIndex = 0; rowIndex < rows; ++rowIndex) {
        swap_complex(ptr1, ptr2);

        ptr1 += cols;
        ptr2 += cols;
    }

    complex_matrix_t new = { values, rows, cols };

    return new;
}
