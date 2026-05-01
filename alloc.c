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
    size_t totalBytes = count * byteSize;

    void* array = malloc(totalBytes);
    check_alloc(array);

    byte_t* startPtr = (byte_t*)array;
    byte_t* endPtr = startPtr + totalBytes;

    for (byte_t* ptr = startPtr; ptr < endPtr; ptr += byteSize) {
        memcpy(ptr, fill, byteSize);
    }

    return array;
}
