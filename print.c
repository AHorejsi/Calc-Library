#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "alloc.h"
#include "error.h"
#include "print.h"


static void do_scan(void* ptr) {
    char response[30];

    if (NULL == fgets(response, sizeof(response), stdin)) {
        fail("\nFailed to read input\n");
    }
    
    char* endPtr;
    uint8_t value = strtoul(response, &endPtr, 10);

    memcpy(ptr, &value, sizeof(uint8_t));
}

void input(const char* prompt, const char* format, void* ptr) {
    printf(prompt);
    do_scan(ptr);
    fflush(stdin);
}

uint8_t input_menu_option(const char* prompt, const uint8_t max) {
    int8_t option;

    while (true) {
        option = -1;

        input(prompt, "%d", &option);
        new_line();

        --option;

        if (0 <= option && option <= max) {
            break;
        }

        printf("ERROR: Input must be between 1 and %u (Actual: %d)\n", max + 1, option + 1);
        new_line();
    }

    return option;
}

void new_line(void) {
    printf("\n");
}

void show(const char* text) {
    printf("%s\n", text);
}
