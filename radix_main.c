#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "alloc.h"
#include "string_utils.h"
#include "print.h"
#include "error.h"
#include "radix.h"
#include "radix_main.h"

#define OPTION_TO 1
#define OPTION_FROM 2

#define OPTION_BINARY 1
#define OPTION_HEX 2
#define OPTION_OCTAL 3

#define DIGITS_BINARY "01"
#define DIGITS_OCTAL "01234567"
#define DIGITS_DECIMAL "0123456789"
#define DIGITS_HEX "0123456789ABCDEF"

#define NAME_DECIMAL "Decimal"
#define NAME_BINARY "Binary"
#define NAME_HEX "Hexadecimal"
#define NAME_OCTAL "Octal"


typedef char*(*conversion_t)(const char*);

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
        name = NAME_BINARY;

        break;
    case HEX:
        name = NAME_HEX;

        break;
    case OCTAL:
        name = NAME_OCTAL;

        break;
    }

    return name;
}

static radix_direction_t choose_radix_direction(const radix_option_t option, const char* name) {
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

static char* determine_prompt(const radix_option_t option, const radix_direction_t direction, const char* name) {
    const char* title = (TO == direction) ? NAME_DECIMAL : name;
    const char* format = "Enter %s Integer: ";

    char temp[35];
    size_t length = sprintf(temp, format, title);

    return copy_to_string(temp, length);
}

static char* find_digit_set(const radix_option_t option, const radix_direction_t direction) {
    if (TO == direction) {
        return DIGITS_DECIMAL;
    }

    char* digitSet;

    switch (option) {
    case BINARY:
        digitSet = DIGITS_BINARY;

        break;
    case HEX:
        digitSet = DIGITS_HEX;

        break;
    case OCTAL:
        digitSet = DIGITS_OCTAL;

        break;
    }

    return digitSet;
}

static bool contains_digit(const char current, const char* digitSet) {
    for (char* digit = (char*)digitSet; *digit != END_CHAR; ++digit) {
        if (current == *digit) {
            return true;
        }
    }

    return false;
}

static bool has_correct_digits(const char* value, const radix_option_t option, const radix_direction_t direction) {
    const char* digitSet = find_digit_set(option, direction);

    for (char* ptr = (char*)value; *ptr != END_CHAR; ++ptr) {
        if (!contains_digit(*ptr, digitSet)) {
            return false;
        }
    }

    return true;
}

static char* input_value(const radix_option_t option, const radix_direction_t direction, const char* name) {
    char value[100];
    char* prompt = determine_prompt(option, direction, name);
    
    input(prompt, "%s", &value);
    size_t length = strlen(value);

    new_line();

    free(prompt);

    if (!has_correct_digits(value, option, direction)) {
        fail("ERROR: Incorrect format for radix\n");
    }

    return copy_to_string(value, length);
}

static char* decide_direction(const char* value, const radix_direction_t direction, const conversion_t to, const conversion_t from) {
    return (TO == direction) ? to(value) : from(value);
}

static char* do_conversion(const char* value, const radix_option_t option, const radix_direction_t direction) {
    char* result;

    switch (option) {
    case BINARY:
        result = decide_direction(value, direction, to_binary, from_binary);

        break;
    case HEX:
        result = decide_direction(value, direction, to_hex, from_hex);

        break;
    case OCTAL:
        result = decide_direction(value, direction, to_octal, from_octal);

        break;
    }

    return result;
}

static void find_radix_names(char* start, char* end, const radix_option_t option, const radix_direction_t direction) {
    char* nonDecimalBaseName = find_name_based_on_option(option);
    
    size_t nonDecimalBytes = (strlen(nonDecimalBaseName) + 1) * sizeof(char);
    size_t decimalBytes = (strlen(NAME_DECIMAL) + 1) * sizeof(char);

    if (FROM == direction) {
        memcpy(start, nonDecimalBaseName, nonDecimalBytes);
        memcpy(end, NAME_DECIMAL, decimalBytes);
    }
    else {
        memcpy(start, NAME_DECIMAL, decimalBytes);
        memcpy(end, nonDecimalBaseName, nonDecimalBytes);
    }
}

static void find_result(const radix_option_t option, const radix_direction_t direction, char* value, const char* name) {
    char* finalResult = do_conversion(value, option, direction);

    char startBaseName[15];
    char endBaseName[15];
    find_radix_names(startBaseName, endBaseName, option, direction);

    printf("Result (%s to %s): %s\n", startBaseName, endBaseName, finalResult);

    free(value);
    free(finalResult);
}

void run_radix(void) {
    radix_option_t option = choose_radix_option();
    const char* name = find_name_based_on_option(option);
    radix_direction_t direction = choose_radix_direction(option, name);
    char* value = input_value(option, direction, name);
    find_result(option, direction, value, name);
}
