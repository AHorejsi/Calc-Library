#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>


typedef char byte_t;

void check_alloc(const void*);

void* falloc(const size_t, const size_t, const void*);


#endif
