#include "http.h"
int main(void) {
    char buf[8193];
    size_t n=fread(buf,1,8192,stdin);
    if(ferror(stdin)||getchar()!=EOF)return fail("input too large or unreadable");
    buf[n]=0;
    Request r;
    if(!parse_http(buf,n,&r))return fail("invalid request");
    printf("method=GET target=%s version=HTTP/1.1 headers=%zu\n",r.target,r.headers);
    return 0;
}
