#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "start.h"
#include "error.h"
#include "print.h"

#define NUMBER_OPTION 1
#define RADIX_OPTION 2
#define UNIT_CONVERT_OPTION 3
#define SHAPE2D_OPTION 4
#define SHAPE3D_OPTION 5
#define STATS_OPTION 6
#define POLYNOMIAL_OPTION 7
#define EXIT_OPTION 8


typedef enum {
    NUMBER,
    RADIX,
    UNIT_CONVERSION,
    SHAPE2D,
    SHAPE3D,
    STATISTICS,
    POLYNOMIAL,
    TERMINATE_PROGRAM
} math_option_t;

static math_option_t choose_math_option(void) {
    static const char* prompt =
        "Choose Option:\n"
        "  Numbers(1)\n"
        "  Radix(2)\n"
        "  Unit Converter(3)\n"
        "  2D Shapes(4)\n"
        "  3D Shapes(5)\n"
        "  Statistics(6)\n"
        "  Polynomial(7)\n"
        "  Exit(8)\n"
        "Enter (1-8): ";
    uint8_t choice;
    input(prompt, "%u", &choice);

    new_line();

    math_option_t option;

    switch (choice) {
    case NUMBER_OPTION:
        option = NUMBER;

        break;
    case RADIX_OPTION:
        option = RADIX;

        break;
    case UNIT_CONVERT_OPTION:
        option = UNIT_CONVERSION;

        break;
    case SHAPE2D_OPTION:
        option = SHAPE2D;

        break;
    case SHAPE3D_OPTION:
        option = SHAPE3D;

        break;
    case STATS_OPTION:
        option = STATISTICS;

        break;
    case POLYNOMIAL_OPTION:
        option = POLYNOMIAL;

        break;
    case EXIT_OPTION:
        option = TERMINATE_PROGRAM;

        break;
    default:
        fail("ERROR: Input must be between 1 and 8\n");
    }

    return option;
}

static void do_math_operation(const math_option_t option) {
    switch (option) {
    case NUMBER:
        fail("Not yet implemented");

        break;
    case RADIX:
        radix_start();

        break;
    case UNIT_CONVERSION:
        fail("Not yet implemented");

        break;
    case SHAPE2D:
        shape2d_start();

        break;
    case SHAPE3D:
        fail("Not yet implemented");

        break;
    case STATISTICS:
        fail("Not yet implemented");

        break;
    case POLYNOMIAL:
        fail("Not yet implemented");

        break;
    default:
        fail("Ya done goofed, Mr. Coder Man\n");
    }
}

int main(void) {
    while (true) {
        math_option_t option = choose_math_option();

        if (TERMINATE_PROGRAM == option) {
            break;
        }

        do_math_operation(option);
        new_line();
    }

    printf("Exiting...\n");

    return EXIT_SUCCESS;
}
