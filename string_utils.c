#include <string.h>
#include "string_utils.h"


const char MIN_DIGIT = '0';
const char MAX_DIGIT = '9';
const char END_CHAR = 0;
const char LINE_FEED = '\n';

bool is_null_or_empty(const char* str) {
    return NULL == str || END_CHAR == str[0];
}

size_t string_length(const char* str) {
    return (NULL == str) ? 0 : strlen(str);
}

const char* string_value(const char* str) {
    if (NULL == str) {
        return "null";
    }
    else if (END_CHAR == str[0]) {
        return "\"\"";
    }
    else {
        return str;
    }
}

bool string_equals(const char* str1, const char* str2) {
    if (NULL == str1 && NULL == str2) {
        return true;
    }
    else if (NULL == str1 ^ NULL == str2) {
        return false;
    }
    else {
        return 0 == strcmp(str1, str2);
    }
}

bool string_contains(const char* str, const char elem) {
    if (NULL == str) {
        return false;
    }

    for (char* ptr = (char*)str; *ptr != END_CHAR; ++ptr) {
        if (elem == *ptr) {
            return true;
        }
    }

    return false;
}
