#include <stddef.h>
#include <stdio.h>
int course_utf8_count(const unsigned char *, size_t, size_t *);
int main(void) {
    size_t n = 999;
    const unsigned char fa[] = {0xd8,0xb3,0xd9,0x84,0xd8,0xa7,0xd9,0x85};
    const unsigned char bad[] = {0xed,0xa0,0x80};
    const unsigned char embedded[] = {'A',0,'B'};
    if (course_utf8_count(fa,sizeof fa,&n) || n!=4) return 1;
    if (course_utf8_count(embedded,sizeof embedded,&n) || n!=3) return 1;
    if (!course_utf8_count(bad,sizeof bad,&n)) return 1;
    if (course_utf8_count(NULL,0,&n) || n!=0) return 1;
    if (!course_utf8_count(NULL,1,&n)) return 1;
    if (!course_utf8_count(NULL,0,NULL)) return 1;
    puts("ffi harness passed");
    return 0;
}
