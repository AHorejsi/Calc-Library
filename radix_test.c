#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "alloc.h"
#include "print.h"
#include "string_utils.h"
#include "radix.h"
#include "radix_test.h"


typedef size_t(*base_convert_t)(const char*, char**);

static bool examine_results(size_t amount, ...) {
    va_list args;
    va_start(args, amount);

    for (size_t index = 0; index < amount; ++index) {
        bool check = va_arg(args, int);

        if (!check) {
            return false;
        }
    }

    va_end(args);

    new_line();

    return true;
}

static bool test_helper(const char* input, const char* expected, const char* name, const base_convert_t baseConvert) {
    size_t expectedLength = string_length(expected);
    
    char* actual;
    size_t actualLength = baseConvert(input, &actual);

    bool isEqualString = string_equals(actual, expected);
    bool isEqualLength = expectedLength == actualLength;

    const char* testResult = isEqualString ? "Success" : "Failure";
    const char* lengthResult = isEqualLength ? "true" : "false";

    const char* inputStr = string_value(input);
    const char* expectedStr = string_value(expected);
    const char* actualStr = string_value(actual);

    printf("%s(Input: %s, Actual: %s, Expected: %s, Result: %s)\n", name, inputStr, actualStr, expectedStr, testResult);
    printf("length(Actual: %u, Expected: %d, Result: %s)\n", actualLength, expectedLength, lengthResult);
    new_line();

    free(actual);
    
    return isEqualString && isEqualLength;
}

static char* all_chars_except(const char* exclusions) {
    size_t length = INT8_MAX - strlen(exclusions) + 2;

    char* set = (char*)malloc(length * sizeof(char));
    check_alloc(set);

    char* ptr = set;
    size_t end = (size_t)(INT8_MAX + 1);

    for (char ch = 1; ch < end; ++ch) {
        if (string_contains(exclusions, ch)) {
            continue;
        }

        *ptr = ch;
        ++ptr;
    }

    *ptr = END_CHAR;

    return set;
}

static bool test_invalid_chars(const char* name, char* validCharSet) {
    char* invalidCharSet = all_chars_except(validCharSet);

    for (char* ptr = invalidCharSet; END_CHAR != *ptr; ++ptr) {
        char input[2];
        sprintf(input, "%c", *ptr);

        bool success = test_helper(input, NULL, name, to_binary);

        if (!success) {
            return false;
        }
    }

    free(invalidCharSet);

    return true;
}

static uint8_t to_binary_test(void) {
    const char* name = "to_binary";

    bool result1 = test_helper("0", "0", name, to_binary);
    bool result2 = test_helper("1234567890", "1001001100101100000001011010010", name, to_binary);
    bool result3 = test_helper("-1234567890", "-1001001100101100000001011010010", name, to_binary);
    bool result4 = test_helper("+1234567890", "+1001001100101100000001011010010", name, to_binary);
    bool result5 = test_helper("9876543210", "1001001100101100000001011011101010", name, to_binary);
    bool result6 = test_helper("-9876543210", "-1001001100101100000001011011101010", name, to_binary);
    bool result7 = test_helper("+9876543210", "+1001001100101100000001011011101010", name, to_binary);
    bool result8 = test_helper("", NULL, name, to_binary);
    bool result9 = test_helper(NULL, NULL, name, to_binary);

    bool success1 = examine_results(9, result1, result2, result3, result4, result5, result6, result7, result8, result9);

    if (!success1) {
        return 0;
    }

    bool success2 = test_invalid_chars(name, "+-0123456789");

    if (!success2) {
        return 0;
    }

    return 1;
}

static uint8_t to_hex_test(void) {
    const char* name = "to_hex";

    bool result1 = test_helper("0", "0", name, to_hex);
    bool result2 = test_helper("1234567890", "499602D2", name, to_hex);
    bool result3 = test_helper("-1234567890", "-499602D2", name, to_hex);
    bool result4 = test_helper("+1234567890", "+499602D2", name, to_hex);
    bool result5 = test_helper("9876543210", "24CB016EA", name, to_hex);
    bool result6 = test_helper("-9876543210", "-24CB016EA", name, to_hex);
    bool result7 = test_helper("+9876543210", "+24CB016EA", name, to_hex);
    bool result8 = test_helper("", NULL, name, to_hex);
    bool result9 = test_helper(NULL, NULL, name, to_hex);

    bool success1 = examine_results(9, result1, result2, result3, result4, result5, result6, result7, result8, result9);

    if (!success1) {
        return 0;
    }

    bool success2 = test_invalid_chars(name, "+-0123456789");

    if (!success2) {
        return 0;
    }

    return 1;
}

static uint8_t to_octal_test(void) {
    const char* name = "to_octal";

    bool result1 = test_helper("0", "0", name, to_octal);
    bool result2 = test_helper("1234567890", "11145401322", name, to_octal);
    bool result3 = test_helper("-1234567890", "-11145401322", name, to_octal);
    bool result4 = test_helper("+1234567890", "+11145401322", name, to_octal);
    bool result5 = test_helper("9876543210", "111454013352", name, to_octal);
    bool result6 = test_helper("-9876543210", "-111454013352", name, to_octal);
    bool result7 = test_helper("+9876543210", "+111454013352", name, to_octal);
    bool result8 = test_helper("", NULL, name, to_octal);
    bool result9 = test_helper(NULL, NULL, name, to_octal);

    bool success1 = examine_results(9, result1, result2, result3, result4, result5, result6, result7, result8, result9);

    if (!success1) {
        return 0;
    }

    bool success2 = test_invalid_chars(name, "+-0123456789");

    if (!success2) {
        return 0;
    }

    return 1;
}

static uint8_t from_binary_test(void) {
    return 0;
}

static uint8_t from_hex_test(void) {
    return 0;
}

static uint8_t from_octal_test(void) {
    return 0;
}

static uint16_t run_all_tests(void) {
    uint16_t successCount = 0;

    //successCount += to_binary_test();
    successCount += to_hex_test();
    //successCount += to_octal_test();

    //successCount += from_binary_test();
    //successCount += from_hex_test();
    //successCount += from_octal_test();

    return successCount;
}

void run_radix_test(void) {
    uint16_t totalTestCount = 6;
    uint16_t successfulTestCount = run_all_tests();

    uint16_t failCount = totalTestCount - successfulTestCount;

    printf("Successful Tests: %u\n", successfulTestCount);
    printf("Failed Tests: %u\n", failCount);
    new_line();
}
