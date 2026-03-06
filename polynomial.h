#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stddef.h>


typedef struct {
    double* coefficients;
    size_t count;
} polynomial_t;

double plug_in(const polynomial_t*, const double);

polynomial_t* factor(const polynomial_t*);

polynomial_t plus(const polynomial_t*, const polynomial_t*);

polynomial_t minus(const polynomial_t*, const polynomial_t*);

polynomial_t multiply(const polynomial_t*, const polynomial_t*);


#endif
