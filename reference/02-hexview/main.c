#include "base.h"
int main(void) {
    unsigned char buf[16];
    size_t n,offset=0;
    while((n=fread(buf,1,sizeof buf,stdin))>0) {
        printf("%08zx:",offset);
        for(size_t i=0;i<n;i++)printf(" %02x",buf[i]);
        putchar('\n');
        if(offset>SIZE_MAX-n)return fail("offset overflow");
        offset+=n;
    }
return ferror(stdin)?fail("read failed"):0;
}
