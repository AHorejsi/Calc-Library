#include <math.h>
#include <stdlib.h>
#include "alloc.h"
#include "polynomial.h"


double plug_in(const polynomial_t* formula, const double var) {
    double result = 0;

    for (size_t index = 0; index < formula->count; ++index) {
        result += formula->coefficients[index] * pow(var, index);
    }

    return result;
}

static polynomial_t linear_operation(const polynomial_t* left, const polynomial_t* right, double(*operation)(const double, const double)) {
    size_t minCount = (left->count < right->count) ? left->count : right->count;
    size_t maxCount = (left->count < right->count) ? right->count : left->count;

    double* coefficients = (double*)malloc(maxCount * sizeof(double));
    check_alloc(coefficients);

    size_t index = 0;

    while (index < minCount) {
        double leftCoef = left->coefficients[index];
        double rightCoef = right->coefficients[index];

        coefficients[index++] = operation(leftCoef, rightCoef);
    }

    while (index < left->count) {
        coefficients[index++] = left->coefficients[index];
    }

    while (index < right->count) {
        coefficients[index++] = right->coefficients[index];
    }

    polynomial_t result = { coefficients, maxCount };

    return result;
}

static double coef_add(const double left, const double right) {
    return left + right;
}

polynomial_t plus(const polynomial_t* left, const polynomial_t* right) {
    return linear_operation(left, right, coef_add);
}

static double coef_subtract(const double left, const double right) {
    return left - right;
}

polynomial_t minus(const polynomial_t* left, const polynomial_t* right) {
    return linear_operation(left, right, coef_subtract);
}

polynomial_t multiply(const polynomial_t* left, const polynomial_t* right) {
    size_t newCount = left->count * right->count;

    double* coefficients = (double*)malloc(newCount * sizeof(double));
    check_alloc(coefficients);

    for (size_t leftIndex = 0; leftIndex < left->count; ++leftIndex) {
        double leftCoef = left->coefficients[leftIndex];

        for (size_t rightIndex = 0; rightIndex < right->count; ++rightIndex) {
            double rightCoef = right->coefficients[rightIndex];
            size_t exponent = leftIndex * rightIndex;

            coefficients[exponent] = leftCoef * rightCoef;
        }
    }

    polynomial_t result = { coefficients, newCount };

    return result;
}
