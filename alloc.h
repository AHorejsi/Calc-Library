#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>


void check_alloc(const void*);

void* falloc(const size_t, const size_t, const void*);


#endif
