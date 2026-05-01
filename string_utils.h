#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>


extern const char MIN_DIGIT;
extern const char MAX_DIGIT;
extern const char END_CHAR;
extern const char LINE_FEED;

bool is_null_or_empty(const char*);

size_t string_length(const char*);

const char* string_value(const char*);

bool string_equals(const char*, const char*);

bool string_contains(const char*, const char);


#endif
