#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testlib.h"


#define test_failed_expected(type, should, is, desc, file, line) { \
    printf("not ok %d - %s\n# %s line %d: Expected " type ", but got " type "\n", ++test, desc, file, line, should, is); \
    errors++; }


static int test = 0, plan = -1, errors = 0;

void _test_failed_message(char *message, char *file, int line) {
    printf("Bail out! %s line %d: %s\n", file, line, message);
    exit(1);
}

static void test_ok(char *desc) {
    printf("ok %d - %s\n", ++test, desc);
}

void test_start(char *desc) {
    // Per default, stdout is block buffered when writing to a file or a pipe.
    // If the test program forks without exec, the contents of the block buffer
    // are duplicated and we get duplicate test output. We avoid this issue by
    // always setting stdout to be line buffered.
    setvbuf(stdout, NULL, _IOLBF, 0);
    printf("Starting test: %s\n", desc);
}

void test_plan(int tests) {
    plan = tests;
    printf("1..%d\n", tests);
}

void _test_equals_int64(int64_t is, int64_t should, char* desc, char* file, int line) {
    if (is == should) {
        test_ok(desc);
    } else {
        test_failed_expected("%"PRIu64, should, is, desc, file, line);
    }
}

void _test_equals_int(int is, int should, char* desc, char* file, int line) {
    if (is == should) {
        test_ok(desc);
    } else {
        test_failed_expected("%d", should, is, desc, file, line);
    }
}

void _test_equals_ptr(void* is, void* should, char* desc, char* file, int line) {
    if (is == should) {
        test_ok(desc);
    } else {
        test_failed_expected("%p", should, is, desc, file, line);
    }
}

void _test_failed() {
    errors++;
}

// escape_string replaces some special characters with C escape sequences.
// It returns the original string if there are no special characters or a new
// string that the caller must free.
static const char* escape_string(const char* string) {
#define SPECIAL_CHARS \
    X('\"') X('\\') X('\a') X('\b') X('\f') X('\n') X('\r') X('\t') X('\v')
    size_t special = 0, len = 0;
    for (const char *s = string; *s; s++) {
        len++;
        switch (*s) {
#define X(c) case c: special++; break;
            SPECIAL_CHARS
#undef X
        }
    }
    if (special == 0) return string;
    char* newstring = malloc(len + special + 1);
    char* s = newstring;
    for (size_t i = 0; i < len; i++) {
        switch (string[i]) {
#define X(c) case c: *s++ = '\\'; *s++ = (#c)[2]; break;
            SPECIAL_CHARS
#undef X
        default: *s++ = string[i];
        }
    }
    *s = 0;
    return newstring;
#undef SPECIAL_CHARS
}

void _test_equals_string(const char* is, const char* should, char* desc, char* file, int line) {
    if (is != NULL && strcmp(is, should) == 0) {
        test_ok(desc);
    } else {
        const char* is_escaped = escape_string(is);
        const char* should_escaped = escape_string(should);
        test_failed_expected("\"%s\"", should_escaped, is_escaped, desc, file, line);
        if (is != is_escaped) free((void*) is_escaped);
        if (should != should_escaped) free((void*) should_escaped);
    }
}

void _test_contains_string(const char* is, const char* should, char* desc, char* file, int line) {
    if (is != NULL && strstr(is, should)) {
        test_ok(desc);
    } else {
        char msg[200];
        snprintf(msg, sizeof(msg), "string to contain %s", should);
        test_failed_expected("%s", msg, is, desc, file, line);
    }
}

void _test_assert(int condition, char* desc, char* file, int line) {
    if (condition) {
        test_ok(desc);
    } else {
        test_failed_expected("%s", "truthy value", "0", desc, file, line);
    }
}

int test_end() {
    int ok = 1;
    if (test != plan) {
        if (plan < 0)
            printf("1..%d\n", test);
        else {
            printf("Expected %d test cases, but %d were executed.\n", plan, test);
            ok = 0;
        }
    } 
    if (errors) {
        printf("%d of %d tests failed. You have errors in your solution, please fix them.\n", errors, test);
        ok = 0;
    }
    if (ok) {
        printf("All %d test cases passed.\n", test);
    }
    return !ok;
}
