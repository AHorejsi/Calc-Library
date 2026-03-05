#include <stdio.h>
#include <stdlib.h>
#include "error.h"


void ensure_positive(const double value, const char* name) {
    if (value <= 0) {
        printf("ERROR: %s must be positive\n", name);
        exit(EXIT_FAILURE);
    }
}

void ensure_greater_than(const uint64_t value, const uint64_t minimum, const char* name) {
    if (value <= minimum) {
        printf("ERROR: %s must be greater than %lf\n", name, minimum);
        exit(EXIT_FAILURE);
    }
}

void fail(const char* message) {
    printf(message);
    exit(EXIT_FAILURE);
}
