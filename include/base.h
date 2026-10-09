#ifndef COURSE_BASE_H
#define COURSE_BASE_H
#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static inline int fail(const char *s)  {
    fprintf(stderr,"error: %s\n",s);
    return 2;
}
static inline bool uint_arg(const char *s, uint64_t max, uint64_t *out)  {
    if (!s || !*s || *s=='-' || *s=='+' || isspace((unsigned char)*s)) return false;
    char *end;
    errno=0;
    unsigned long long x=strtoull(s,&end,10);
    if(errno || *end || x>max) return false;
    *out=(uint64_t)x;
    return true;
}
static inline bool int_arg(const char *s, int64_t *out)  {
    if(!s || !*s || isspace((unsigned char)*s)) return false;
    char *end;
    errno=0;
    intmax_t x=strtoimax(s,&end,10);
    if(errno || *end || x<INT64_MIN || x>INT64_MAX) return false;
    *out=(int64_t)x;
    return true;
}
static inline bool ascii_letter(unsigned char c)  {
    return (c>='A' && c<='Z') || (c>='a' && c<='z');
}
static inline unsigned char ascii_lower(unsigned char c)  {
    return c>='A' && c<='Z' ? (unsigned char)(c+32) : c;
}
#endif
