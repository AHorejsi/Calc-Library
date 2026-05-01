#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "alloc.h"
#include "error.h"
#include "string_utils.h"
#include "print.h"

#define INPUT_SIZE 30


static void get_input_string(char* response, const size_t inputSize) {
    if (NULL == fgets(response, inputSize, stdin)) {
        fail("\nFailed to read user input\n");
    }

    size_t endIndex = strlen(response) - 1;

    if (LINE_FEED == response[endIndex]) {
        response[endIndex] = END_CHAR;
    }
}

static void do_scan(const input_format_t format, void* ptr) {
    char response[INPUT_SIZE];
    get_input_string(response, INPUT_SIZE);

    char* _;

    switch (format) {
    case WHOLE:
        *((uint64_t*)ptr) = strtoull(response, &_, 10);

        break;
    case INTEGER:
        *((int64_t*)ptr) = strtoll(response, &_, 10);

        break;
    case DECIMAL:
        *((double*)ptr) = strtod(response, &_);

        break;
    case STRING:
        strcpy((char*)ptr, response);

        break;
    case MENU:
        *((int8_t*)ptr) = atoi(response);

        break;
    }
}

void input(const char* prompt, const input_format_t format, void* ptr) {
    printf(prompt);
    do_scan(format, ptr);
    fflush(stdin);
}

uint8_t input_menu_option(const char* prompt, const uint8_t max) {
    int8_t option;

    while (true) {
        option = -1;

        input(prompt, MENU, &option);
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
