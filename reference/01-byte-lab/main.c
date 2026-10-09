#include "base.h"
int main(int argc,char **argv)  {
    uint64_t n;
    if(argc!=2 || !uint_arg(argv[1],UINT32_MAX,&n)) return fail("expected uint32");
    uint32_t x=(uint32_t)n;
    char bits[33];
    unsigned ones=0;
    for(unsigned i=0;i<32;i++) {
        unsigned b=(x>>(31-i))&1U;
        bits[i]=(char)('0'+b);
        ones+=b;
    }     bits[32]=0;
    printf("dec=%" PRIu32 " hex=%08" PRIX32 " bits=%s ones=%u\n",x,x,bits,ones);
    return 0;
}
