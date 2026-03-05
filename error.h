#ifndef ENSURE_H
#define ENSURE_H

#include <stdint.h>


void ensure_positive(const double, const char*);

void ensure_greater_than(const uint64_t, const uint64_t, const char*);

void fail(const char*);


#endif
