#ifndef TESTLIB_H
#define TESTLIB_H

#include <inttypes.h>
void test_start(char *desc);

void test_plan(int tests);

#define test_equals_int(is, should, desc) _test_equals_int(is, should, desc, __FILE__, __LINE__);
void _test_equals_int(int is, int should, char* desc, char* file, int line);

#define test_equals_int64(is, should, desc) _test_equals_int64(is, should, desc, __FILE__, __LINE__);
void _test_equals_int64(int64_t is, int64_t should, char* desc, char* file, int line);

#define test_equals_ptr(is, should, desc) _test_equals_ptr(is, should, desc, __FILE__, __LINE__);
void _test_equals_ptr(void* is, void* should, char* desc, char* file, int line);

#define test_equals_string(is, should, desc) _test_equals_string(is, should, desc, __FILE__, __LINE__);
void _test_equals_string(const char* is, const char* should, char* desc, char* file, int line);

#define test_contains_string(is, should, desc) _test_contains_string(is, should, desc, __FILE__, __LINE__);
void _test_contains_string(const char* is, const char* should, char* desc, char* file, int line);

#define test_assert(condition, desc) _test_assert(!!(condition), desc, __FILE__, __LINE__);
void _test_assert(int condition, char* desc, char* file, int line);

#define test_failed_message(message) _test_failed_message(message, __FILE__, __LINE__);
void _test_failed_message(char *message, char *file, int line);

int test_end();

#endif
