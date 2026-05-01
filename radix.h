#ifndef RADIX_H
#define RADIX_H


extern const char NEG_SIGN;

size_t to_binary(const char*, char**);

size_t to_twos_comp_binary(const char*, char**);

size_t to_float_binary(const char*, char**);

size_t to_real_binary(const char*, char**);

size_t to_hex(const char*, char**);

size_t to_real_hex(const char*, char**);

size_t to_octal(const char*, char**);

size_t to_real_octal(const char*, char**);

size_t from_binary(const char*, char**);

size_t from_twos_comp_binary(const char*, char**);

size_t from_float_binary(const char*, char**);

size_t from_real_binary(const char*, char**);

size_t from_hex(const char*, char**);

size_t from_real_hex(const char*, char**);

size_t from_octal(const char*, char**);

size_t from_real_octal(const char*, char**);


#endif
