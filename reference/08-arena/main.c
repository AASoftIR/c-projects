#include "base.h"
typedef union  {
    max_align_t alignment;
    unsigned char bytes[4096];
} Storage;
int main(int argc,char **argv) {
    Storage store= {
        0
    };
    size_t used=0;
    const size_t align=_Alignof(max_align_t);
    if(argc<2)return fail("sizes required");
    for(int i=1;i<argc;i++) {
        if(!strcmp(argv[i],"reset")) {
            used=0;
            puts("reset");
            continue;
        }uint64_t n;
        if(!uint_arg(argv[i],sizeof store.bytes,&n)||!n)return fail("invalid size");
        size_t pad=(align-used%align)%align;
        if(pad>sizeof store.bytes-used)return fail("arena exhausted");
        size_t start=used+pad;
        if(n>sizeof store.bytes-start)return fail("arena exhausted");
        memset(store.bytes+start,0,(size_t)n);
        used=start+(size_t)n;
        printf("%zu\n",start);
    }
return 0;
}
