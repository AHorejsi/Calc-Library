#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"
#include "print.h"
#include "error.h"
#include "radix.h"
#include "radix_main.h"

#define OPTION_TO 1
#define OPTION_FROM 2

#define OPTION_BINARY 1
#define OPTION_HEX 2
#define OPTION_OCTAL 3


typedef enum {
    TO,
    FROM
} radix_direction_t;

typedef enum {
    BINARY,
    HEX,
    OCTAL
} radix_option_t;

static radix_option_t choose_radix_option(void) {
    static const char* prompt =
        "Choose Option:\n"
        "  Binary(1)\n"
        "  Hexadecimal(2)\n"
        "  Octal(3)\n"
        "Enter (1-3): ";
    uint8_t choice;
    input(prompt, "%u", &choice);

    new_line();

    radix_option_t option;

    switch (choice) {
    case OPTION_BINARY:
        option = BINARY;

        break;
    case OPTION_HEX:
        option = HEX;

        break;
    case OPTION_OCTAL:
        option = OCTAL;

        break;
    default:
        fail("ERROR: Input must be between 1 and 3\n");
    }

    return option;
}

static char* find_name_based_on_option(const radix_option_t option) {
    char* name;

    switch (option) {
    case BINARY:
        name = "Binary";

        break;
    case HEX:
        name = "Hexadecimal";

        break;
    case OCTAL:
        name = "Octal";

        break;
    }

    return name;
}

static radix_direction_t choose_radix_direction(const radix_option_t option) {
    const char* name = find_name_based_on_option(option);
    static const char* format =
        "Choose Option:\n"
        "  To %s\n"
        "  From %s\n"
        "Enter (1-2): ";
    char prompt[70];
    sprintf(prompt, format, name, name);

    uint8_t choice;
    input(prompt, "%u", &choice);

    new_line();

    radix_direction_t direction;

    switch (choice) {
    case OPTION_TO:
        direction = TO;

        break;
    case OPTION_FROM:
        direction = FROM;

        break;
    default:
        fail("ERROR: Input must be between 1 and 2\n");
    }

    return direction;
}

static char* copy_to_string(const char* str, const size_t length) {
    size_t byteCount = (length + 1) * sizeof(char);

    char* prompt = (char*)malloc(byteCount);
    check_alloc(prompt);

    memcpy(prompt, str, byteCount);

    return prompt;
}

static char* determine_prompt(const radix_option_t option, const radix_direction_t direction) {
    char temp[35];
    size_t length;

    switch (direction) {
    case TO:
        length = sprintf(temp, "Enter Decimal Integer: ");
        
        break;
    case FROM:
        const char* format = "Enter %s Integer: ";
        const char* name = find_name_based_on_option(option);

        length = sprintf(temp, format, name);

        break;
    }

    return copy_to_string(temp, length);
}

static char* input_value(const radix_option_t option, const radix_direction_t direction) {
    char value[30];
    char* prompt = determine_prompt(option, direction);
    
    input(prompt, "%s", &value);
    size_t length = strlen(value);

    new_line();

    free(prompt);

    return copy_to_string(value, length);
}

static char* do_binary(const radix_direction_t direction, const char* value) {
    if (TO == direction) {
        return to_binary(value);
    }
    else {
        return from_binary(value);
    }
}

static char* do_hex(const radix_direction_t direction, const char* value) {
    if (TO == direction) {
        return to_hex(value);
    }
    else {
        return from_hex(value);
    }
}

static char* do_octal(const radix_direction_t direction, const char* value) {
    if (TO == direction) {
        return to_octal(value);
    }
    else {
        return from_octal(value);
    }
}

static void find_result(const radix_option_t option, const radix_direction_t direction, char* value) {
    char* finalResult;

    switch (option) {
    case BINARY:
        finalResult = do_binary(direction, value);

        break;
    case HEX:
        finalResult = do_hex(direction, value);

        break;
    case OCTAL:
        finalResult = do_octal(direction, value);

        break;
    }

    printf("Result: %s\n", finalResult);

    free(value);
    free(finalResult);
}

void run_radix(void) {
    radix_option_t option = choose_radix_option();
    radix_direction_t direction = choose_radix_direction(option);
    char* value = input_value(option, direction);
    find_result(option, direction, value);
}
