#include "utf8.h"
int course_utf8_count(const unsigned char *data,size_t len,size_t *out) {
    return utf8_count(data,len,out);
}
