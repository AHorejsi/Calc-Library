#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>


typedef enum {
    WHOLE,
    INTEGER,
    DECIMAL,
    STRING,
    MENU
} input_format_t;

void input(const char*, const input_format_t, void*);

uint8_t input_menu_option(const char*, const uint8_t);

void new_line(void);

void show(const char*);


#endif
