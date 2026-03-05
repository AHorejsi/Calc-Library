#include <stdio.h>
#include <stdlib.h>
#include "print.h"


void input(const char* prompt, const char* format, void* ptr) {
    printf(prompt);
    scanf(format, ptr);
    fflush(stdin);
}

void new_line(void) {
    printf("\n");
}
