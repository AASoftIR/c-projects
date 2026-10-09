#include "base.h"
#include "utf8.h"
int main(void) {
    size_t len=0,cap=256;
    unsigned char *p=malloc(cap);
    if(!p)return fail("allocation");
    int c;
    while((c=getchar())!=EOF) {
        if(len==cap) {
            if(cap>SIZE_MAX/2) {
                free(p);
                return fail("too large");
            }cap*=2;
            void *q=realloc(p,cap);
            if(!q) {
                free(p);
                return fail("allocation");
            }p=q;
        }p[len++]=(unsigned char)c;
    }
size_t count;
    int ok=!ferror(stdin) && utf8_count(p,len,&count)==0;
    free(p);
    if(!ok)return fail("invalid UTF-8 or read error");
    printf("bytes=%zu codepoints=%zu\n",len,count);
    return 0;
}
