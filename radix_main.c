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
    
    return input_menu_option(prompt, OCTAL);
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

    return input_menu_option(prompt, FROM);
}

static void determine_prompt(char* prompt, const radix_direction_t direction, const char* name) {
    const char* title = (TO == direction) ? NAME_DECIMAL : name;

    sprintf(prompt, "Enter %s Integer: ", title);
}

static void find_digit_set(const radix_option_t option, const radix_direction_t direction, char* digitSet) {
    if (TO == direction) {
        strcpy(digitSet, DIGITS_DECIMAL);
    }
    else {
        switch (option) {
        case BINARY:
            strcpy(digitSet, DIGITS_BINARY);

            break;
        case HEX:
            strcpy(digitSet, DIGITS_HEX);

            break;
        case OCTAL:
            strcpy(digitSet, DIGITS_OCTAL);

            break;
        }
    }
}

static bool contains_digit(const char current, const char* digitSet) {
    for (char* digit = (char*)digitSet; *digit != END_CHAR; ++digit) {
        if (current == *digit) {
            return true;
        }
    }

    return false;
}

static const char* find_first_digit(const char* value) {
    if (NEG_SIGN == value[0]) {
        return value + 1;
    }
    else {
        return value;
    }
}

static bool has_correct_digits(const char* value, const radix_option_t option, const radix_direction_t direction) {
    char digitSet[20];
    find_digit_set(option, direction, digitSet);

    for (char* ptr = (char*)find_first_digit(value); *ptr != END_CHAR; ++ptr) {
        if (!contains_digit(*ptr, digitSet)) {
            return false;
        }
    }

    return true;
}

static char* input_value(const radix_option_t option, const radix_direction_t direction, const char* name) {
    char prompt[40];
    determine_prompt(prompt, direction, name);
    
    char* value = (char*)malloc(100 * sizeof(char));
    check_alloc(value);

    input(prompt, STRING, value);
    new_line();

    if (!has_correct_digits(value, option, direction)) {
        fail("ERROR: Incorrect format for radix\n");
    }

    return value;
}

static char* decide_direction(const char* value, const radix_direction_t direction, const conversion_t to, const conversion_t from) {
    return (FROM == direction) ? from(value) : to(value);
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

    char* startName;
    char* endName;

    size_t startBytes;
    size_t endBytes;

    if (FROM == direction) {
        startName = nonDecimalBaseName;
        startBytes = nonDecimalBytes;

        endName = NAME_DECIMAL;
        endBytes = decimalBytes;
    }
    else {
        endName = nonDecimalBaseName;
        endBytes = nonDecimalBytes;

        startName = NAME_DECIMAL;
        startBytes = decimalBytes;
    }

    memcpy(start, startName, startBytes);
    memcpy(end, endName, endBytes);
}

static void find_result(const radix_option_t option, const radix_direction_t direction, char* value, const char* name) {
    char* finalResult = do_conversion(value, option, direction);

    char startBaseName[15];
    char endBaseName[15];
    find_radix_names(startBaseName, endBaseName, option, direction);

    printf("Result (%s -> %s): %s\n", startBaseName, endBaseName, finalResult);
    new_line();

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
