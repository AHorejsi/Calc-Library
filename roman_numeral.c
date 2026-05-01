#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "string_utils.h"
#include "alloc.h"
#include "roman_numeral.h"

#define ONES_PLACE 1
#define TENS_PLACE 10
#define HUNDREDS_PLACE 100
#define THOUSANDS_PLACE 1000

#define THOUSAND_CHAR 'M'
#define FIVE_HUNDER_CHAR 'D'
#define FOUR_HUNDRED_STR "CD"
#define NINE_HUNDRED_STR "CM"
#define HUNDRED_CHAR 'C'
#define FIFTY_CHAR 'L'
#define FORTY_STR "XL"
#define NINETY_STR "XC"
#define TEN_CHAR 'X'
#define FIVE_CHAR 'V'
#define FOUR_STR "IV"
#define NINE_STR "IX"
#define ONE_CHAR 'I'


static size_t places(uint16_t* value, const size_t powerOfTen) {
    size_t place = *value / powerOfTen;
    *value -= place * powerOfTen;

    return place;
}

static size_t amount_of_places(uint16_t* value, const size_t powerOfTen) {
    if (*value >= powerOfTen) {
        return 0;
    }

    size_t place = places(value, powerOfTen);

    size_t length;

    switch (place) {
    case 4:
    case 9:
        length = 2;

        break;
    default:
        length = 0;

        if (place >= 5) {
            ++length;
            place -= 5;
        }

        while (place > 0) {
            ++length;
            --place;
        }
    }

    return length;
}

static size_t determine_length(uint16_t value) {
    size_t length = 0;

    length += amount_of_places(&value, THOUSANDS_PLACE);
    length += amount_of_places(&value, HUNDREDS_PLACE);
    length += amount_of_places(&value, TENS_PLACE);
    length += amount_of_places(&value, ONES_PLACE);

    return length;
}

static void thousands_places_chars(uint16_t* value, char** roman) {
    size_t place = places(value, THOUSANDS_PLACE);
    char* ptr = *roman;

    for (size_t count = 0; count < place; ++count) {
        *ptr = THOUSAND_CHAR;
        ++ptr;
    }

    *roman = ptr;
}

static void hundreds_places_chars(uint16_t* value, char** roman) {
    size_t place = places(value, HUNDREDS_PLACE);

    if (0 == place) {
        return;
    }

    char* ptr = *roman;

    switch (place) {
    case 4:
        *(ptr++) = FOUR_HUNDRED_STR[0];
        *(ptr++) = FOUR_HUNDRED_STR[1];

        break;
    case 9:
        *(ptr++) = NINE_HUNDRED_STR[0];
        *(ptr++) = NINE_HUNDRED_STR[1];

        break;
    default:
        if (place > 5) {
            *(ptr++) = FIVE_HUNDER_CHAR;
            place -= 5;
        }

        while (place > 0) {
            *(ptr++) = HUNDRED_CHAR;
            --place;
        }
    }

    *roman = ptr;
}

static void tens_places_chars(uint16_t* value, char** roman) {
    size_t place = places(value, TENS_PLACE);

    if (0 == place) {
        return;
    }

    char* ptr = *roman;

    switch (place) {
    case 4:
        *(ptr++) = FORTY_STR[0];
        *(ptr++) = FORTY_STR[1];

        break;
    case 9:
        *(ptr++) = NINETY_STR[0];
        *(ptr++) = NINETY_STR[1];

        break;
    default:
        if (place > 5) {
            *(ptr++) = FIFTY_CHAR;
            place -= 5;
        }

        while (place > 0) {
            *(ptr++) = TEN_CHAR;
            --place;
        }
    }

    *roman = ptr;
}

static void ones_places_chars(uint16_t* value, char** roman) {
    if (0 == *value) {
        return;
    }

    char* ptr = *roman;

    switch (*value) {
    case 4:
        *(ptr++) = FOUR_STR[0];
        *(ptr++) = FOUR_STR[1];

        break;
    case 9:
        *(ptr++) = NINE_STR[0];
        *(ptr++) = NINE_STR[1];

        break;
    default:
        if (*value > 5) {
            *(ptr++) = FIVE_CHAR;
            *value -= 5;
        }

        while (*value > 0) {
            *(ptr++) = ONE_CHAR;
            --(*value);
        }
    }
}

char* to_roman(const uint16_t value) {
    size_t length = determine_length(value) + 1;
    uint16_t num = value;

    char* roman = (char*)malloc(length * sizeof(char));
    check_alloc(roman);

    char* ptr = roman;
    thousands_places_chars(&num, &ptr);
    hundreds_places_chars(&num, &ptr);
    tens_places_chars(&num, &ptr);
    ones_places_chars(&num, &ptr);
    *ptr = END_CHAR;

    return roman;
}

static int16_t find_value_with(const char digit, const char fourVal, const char nineChar) {
    int16_t factor = -1;

    if (digit == fourVal) {
        factor = 4;
    }
    else if (digit == nineChar) {
        factor = 9;
    }

    return factor;
}

static int16_t find_value_from_string_of_two(char* sub) {
    int16_t value = -1;

    switch (sub[0]) {
    case ONE_CHAR:
        value = find_value_with(sub[1], FIVE_CHAR, TEN_CHAR);

        break;
    case TEN_CHAR:
        value = TENS_PLACE * find_value_with(sub[1], FIFTY_CHAR, HUNDRED_CHAR);

        break;
    case HUNDRED_CHAR:
        value = HUNDREDS_PLACE * find_value_with(sub[1], FIVE_HUNDER_CHAR, THOUSAND_CHAR);

        break;
    }

    return value;
}

static int16_t find_value_from_string_of_one(const char digit) {
    int16_t value = -1;

    switch (digit) {
    case ONE_CHAR:
        value = ONES_PLACE;

        break;
    case FIVE_CHAR:
        value = 5;

        break;
    case TEN_CHAR:
        value = TENS_PLACE;

        break;
    case FIFTY_CHAR:
        value = 50;

        break;
    case HUNDRED_CHAR:
        value = HUNDREDS_PLACE;

        break;
    case FIVE_HUNDER_CHAR:
        value = 500;

        break;
    case THOUSAND_CHAR:
        value = THOUSANDS_PLACE;

        break;
    }

    return value;
}

uint16_t from_roman(const char* roman) {
    char* ptr = roman;
    uint16_t result = 0;

    while (END_CHAR != *ptr) {
        char current = *ptr;
        char next = *(ptr + 1);

        if (END_CHAR != next) {
            char twoDigits[] = { current, next };
            int16_t value = find_value_from_string_of_two(twoDigits);

            if (-1 != value) {
                result += value;
                ptr += 2;

                continue;
            }
        }

        result += find_value_from_string_of_one(current);
        ++ptr;
    }

    return result;
}
