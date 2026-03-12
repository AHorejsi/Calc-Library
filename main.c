#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "print.h"
#include "error.h"
#include "start.h"


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
        radix_start();

        break;
    case FACTORIZE:
        fail("Not yet implemented");

        break;
    case UNIT_CONVERSION:
        fail("Not yet implemented");

        break;
    case SHAPE2D:
        shape2d_start();

        break;
    case STATISTICS:
        fail("Not yet implemented");

        break;
    case POLYNOMIAL:
        fail("Not yet implemented");

        break;
    default:
        fail("Ya done goofed, Mr. Coder Man");
    }
}

int main(void) {
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
        "  Exit(10)\n"
        "Enter (1-10): ";
    
    while (true) {
        math_option_t option = input_menu_option(prompt, TERMINATION);

        if (TERMINATION == option) {
            break;
        }

        do_operation(option);
    }

    show("Exiting...");
    new_line();

    return EXIT_SUCCESS;
}
