#include <stdlib.h>
#include <string.h>
#include "print.h"
#include "error.h"
#include "alloc.h"


void check_alloc(const void* ptr) {
    if (NULL == ptr) {
        fail("Allocation Failed");
    }
}

void* falloc(const size_t count, const size_t byteSize, const void* fill) {
    void* array = malloc(count * byteSize);
    check_alloc(array);

    char* startPtr = (char*)array;
    char* endPtr = startPtr + (count * byteSize);

    for (char* ptr = startPtr; ptr < endPtr; ptr += byteSize) {
        memcpy(ptr, fill, byteSize);
    }

    return array;
}
