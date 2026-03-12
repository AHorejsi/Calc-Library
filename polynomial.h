#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stddef.h>


typedef struct {
    double* coefficients;
    size_t count;
} polynomial_t;

typedef struct {
    double* numerator;
    polynomial_t* denominator;
} term_t;
    
typedef struct {
    term_t* terms;
    size_t count;
} formula_t;

double plug_in(const polynomial_t*, const double);

polynomial_t* factor(const polynomial_t*);

polynomial_t polynomial_plus(const polynomial_t*, const polynomial_t*);

polynomial_t polynomial_minus(const polynomial_t*, const polynomial_t*);

polynomial_t polynomial_mult(const polynomial_t*, const polynomial_t*);

formula_t polynomial_div(const polynomial_t*, const polynomial_t*);

polynomial_t derive(const polynomial_t*);

polynomial_t integrate(const polynomial_t*);


#endif
