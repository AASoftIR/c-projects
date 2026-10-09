#ifndef COURSE_UTF8_H
#define COURSE_UTF8_H
#include <stddef.h>
#include <stdint.h>
/* Scalar validation only: neither Unicode normalization nor grapheme counting. */ static inline int utf8_count(const unsigned char *s,size_t n,size_t *out)  {
    if(!out || (!s && n)) return -1;
    size_t i=0,count=0;
    while(i<n)  {
        uint32_t v=s[i++],min=0;
        unsigned need=0;
        if(v<0x80)  {
            count++;
            continue;
        }
if(v>=0xc2 && v<=0xdf)  {
            need=1;
            v&=31;
            min=0x80;
        }         else if(v>=0xe0 && v<=0xef)  {
            need=2;
            v&=15;
            min=0x800;
        }         else if(v>=0xf0 && v<=0xf4)  {
            need=3;
            v&=7;
            min=0x10000;
        }         else return -1;
        if(n-i<need) return -1;
        for(unsigned j=0;j<need;j++)  {
            unsigned c=s[i++];
            if((c&0xc0)!=0x80)return -1;
            v=(v<<6)|(c&63);
        }
if(v<min || v>0x10ffff || (v>=0xd800 && v<=0xdfff)) return -1;
        count++;
    }     *out=count;
    return 0;
}
#endif
