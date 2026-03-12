#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "error.h"


void ensure_positive(const double value, const char* name) {
    if (value <= 0) {
        printf("ERROR: %s must be positive\n", name);
        exit(EXIT_FAILURE);
    }
}

void ensure_greater_than(const uint64_t value, const uint64_t minimum, const char* name) {
    if (value <= minimum) {
        printf("ERROR: %s must be greater than %u\n", name, minimum);
        exit(EXIT_FAILURE);
    }
}

void fail(const char* text) {
    show(text);
    exit(EXIT_FAILURE);
}
