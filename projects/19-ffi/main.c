/* 19-ffi: Python ↔ C Bridge
 * Contract: libcourse.so exports int course_utf8_count(const unsigned char*,size_t,size_t*)
 * 1. Write the input and failure contract.
 * 2. Identify ownership and invariants.
 * 3. Build a happy path, then edge cases.
 * Guide: README.md; tests: ../../tests/cases.py
 */
#include <stddef.h>
int course_utf8_count(const unsigned char *data, size_t len, size_t *out) {
    (void)data; (void)len; (void)out;
    return -1; /* TODO */
}
