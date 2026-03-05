#include <stdlib.h>
#include <stdio.h>
#include "alloc.h"


void check_alloc(const void* ptr) {
    if (NULL == ptr) {
        printf("Allocation Failed...\n");

        exit(EXIT_FAILURE);
    }
}
