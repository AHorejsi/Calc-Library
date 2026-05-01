#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "print.h"
#include "error.h"
#include "start.h"


typedef enum {
    MAIN,
    TEST,
    KILL
} program_option_t;

typedef enum {
    RATIONAL,
    COMPLEX,
    ROUNDING,
    RADIX,
    FACTORIZE,
    UNIT_CONVERSION,
    SHAPE2D,
    STATISTICS,
    POLYNOMIAL,
    VECTOR,
    TERMINATION
} math_option_t;

static void do_operation(const math_option_t option) {
    switch (option) {
    case RATIONAL:
        fail("Not yet implemented");

        break;
    case COMPLEX:
        fail("Not yet implemented");

        break;
    case ROUNDING:
        fail("Not yet implemented");

        break;
    case RADIX:
        fail("Not yet implemented");

        break;
    case FACTORIZE:
        fail("Not yet implemented");

        break;
    case UNIT_CONVERSION:
        fail("Not yet implemented");

        break;
    case SHAPE2D:
        fail("Not yet implemented");

        break;
    case STATISTICS:
        fail("Not yet implemented");

        break;
    case POLYNOMIAL:
        fail("Not yet implemented");

        break;
    case VECTOR:
        fail("Not yet implemented");

        break;
    default:
        fail("Ya done goofed, Mr. Coder Man\n");
    }
}

static void select_program(void) {   
    const char* prompt =
        "Choose Option:\n"
        "  Rational(1)\n"
        "  Complex(2)\n"
        "  Rounding(3)\n"
        "  Radix(4)\n"
        "  Factorize(5)\n"
        "  Unit Converter(6)\n"
        "  2D Shape(7)\n"
        "  Statistics(8)\n"
        "  Polynomial(9)\n"
        "  Vector(10)\n"
        "  Exit(11)\n"
        "Enter (1-11): ";
    
    while (true) {
        math_option_t option = input_menu_option(prompt, TERMINATION);

        if (TERMINATION == option) {
            break;
        }

        do_operation(option);
    }
}

static void do_test(const math_option_t option) {
    switch (option) {
    case RATIONAL:
        fail("Not yet implemented");

        break;
    case COMPLEX:
        fail("Not yet implemented");

        break;
    case ROUNDING:
        fail("Not yet implemented");

        break;
    case RADIX:
        radix_test();

        break;
    case FACTORIZE:
        fail("Not yet implemented");

        break;
    case UNIT_CONVERSION:
        fail("Not yet implemented");

        break;
    case SHAPE2D:
        fail("Not yet implemented");

        break;
    case STATISTICS:
        fail("Not yet implemented");

        break;
    case POLYNOMIAL:
        fail("Not yet implemented");

        break;
    case VECTOR:
        fail("Not yet implemented");

        break;
    default:
        fail("Ya done goofed, Mr. Coder Man\n");
    }
}

static void select_test(void) {
    const char* prompt =
        "Choose Option:\n"
        "  Rational(1)\n"
        "  Complex(2)\n"
        "  Rounding(3)\n"
        "  Radix(4)\n"
        "  Factorize(5)\n"
        "  Unit Converter(6)\n"
        "  2D Shape(7)\n"
        "  Statistics(8)\n"
        "  Polynomial(9)\n"
        "  Vector(10)\n"
        "  Exit(11)\n"
        "Enter (1-11): ";

    while (true) {
        math_option_t option = input_menu_option(prompt, TERMINATION);

        if (TERMINATION == option) {
            break;
        }

        do_test(option);
    }
}

int main(void) {   
    const char* prompt =
        "Choose Option:\n"
        "  Go to Main Program(1)\n"
        "  Test Features(2)\n"
        "  End Program(3)\n"
        "Enter (1-3): ";

    program_option_t option = input_menu_option(prompt, KILL);

    switch (option) {
    case MAIN:
        select_program();

        break;
    case TEST:
        select_test();

        break;
    case KILL:
        show("Terminating...");
        new_line();

        break;
    default:
        fail("Ya done goofed, Mr. Coder Man\n");
    }

    show("Exiting...");
    new_line();

    return EXIT_SUCCESS;
}
