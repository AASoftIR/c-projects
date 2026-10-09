#ifndef COURSE_HTTP_H
#define COURSE_HTTP_H
#include "base.h"
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <signal.h>
typedef struct  {
    char target[2048];
    size_t headers;
} Request;
static inline bool header_token(unsigned char c) {
    return ascii_letter(c)||(c>='0'&&c<='9')||strchr("!#$%&'*+-.^_`|~",c)!=NULL;
}
static inline bool equal_fold(const char *a,const char *b) {
    while(*a&&*b) {
        if(ascii_lower((unsigned char)*a++)!=ascii_lower((unsigned char)*b++))return false;
    }
return *a==*b;
} /* Mutable, NUL-terminated complete header block; body data must not be present. */ static inline bool parse_http(char *buf,size_t n,Request *r) {
    if(n>8192 || n<4 || memchr(buf,0,n) || memcmp(buf+n-4,"\r\n\r\n",4))return false;
    char *first=strstr(buf,"\r\n");
    if(!first)return false;
    *first=0;
    if(strncmp(buf,"GET ",4))return false;
    char *target=buf+4,*space=strchr(target,' ');
    if(!space || strcmp(space," HTTP/1.1"))return false;
    *space=0;
    size_t len=strlen(target);
    if(!len || target[0]!='/' || len>=sizeof r->target)return false;
    for(size_t i=0;i<len;i++)if((unsigned char)target[i]<=32||(unsigned char)target[i]>=127)return false;
    memcpy(r->target,target,len+1);
    r->headers=0;
    unsigned hosts=0,lengths=0;
    char *p=first+2;
    while(*p) {
        char *end=strstr(p,"\r\n");
        if(!end)return false;
        *end=0;
        if(!*p)break;
        char *colon=strchr(p,':');
        if(!colon||colon==p)return false;
        for(char *q=p;q<colon;q++)if(!header_token((unsigned char)*q))return false;
        *colon=0;
        char *value=colon+1;
        while(*value==' '||*value=='\t')value++;
        for(char *q=value;*q;q++)if(((unsigned char)*q<32&&*q!='\t')||(unsigned char)*q>=127)return false;
        size_t vl=strlen(value);
        while(vl&&(value[vl-1]==' '||value[vl-1]=='\t'))value[--vl]=0;
        if(equal_fold(p,"Host")) {
            if(++hosts>1 || !vl)return false;
        }
if(equal_fold(p,"Transfer-Encoding"))return false;
        if(equal_fold(p,"Content-Length")) {
            if(++lengths>1 || strcmp(value,"0"))return false;
        }
r->headers++;
        p=end+2;
    }
return hosts==1;
}
static inline bool send_all(int fd,const char *s,size_t n) {
    while(n) {
        ssize_t k=send(fd,s,n,0);
        if(k<0&&errno==EINTR)continue;
        if(k<=0)return false;
        s+=k;
        n-=(size_t)k;
    }
return true;
}
static inline void response(int fd,int code,const char *body) {
    const char *reason=code==200?"OK":code==400?"Bad Request":code==404?"Not Found":"Internal Server Error";
    char head[256];
    int n=snprintf(head,sizeof head,"HTTP/1.1 %d %s\r\nContent-Type: application/json; charset=utf-8\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n",code,reason,strlen(body));
    if(n>0&&(size_t)n<sizeof head&&send_all(fd,head,(size_t)n))(void)send_all(fd,body,strlen(body));
}
static inline int listen_loopback(unsigned port) {
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0)return -1;
    int yes=1;
    (void)setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes);
    struct sockaddr_in addr= {
        0
    };
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    addr.sin_port=htons((uint16_t)port);
    if(bind(fd,(struct sockaddr*)&addr,sizeof addr)||listen(fd,16)) {
        close(fd);
        return -1;
    }socklen_t len=sizeof addr;
    if(getsockname(fd,(struct sockaddr*)&addr,&len)) {
        close(fd);
        return -1;
    }printf("READY=%u\n",(unsigned)ntohs(addr.sin_port));
    fflush(stdout);
    return fd;
}
static inline bool receive_request(int fd,Request *r) {
    struct timeval timeout= {
        2,0
    };
    (void)setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof timeout);
    (void)setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof timeout);
    char buf[8193];
    size_t n=0;
    while(n<8192) {
        ssize_t k=recv(fd,buf+n,8192-n,0);
        if(k<0&&errno==EINTR)continue;
        if(k<=0)return false;
        n+=(size_t)k;
        buf[n]=0;
        if(memchr(buf,0,n))return false;
        if(strstr(buf,"\r\n\r\n"))return parse_http(buf,n,r);
    }
return false;
}
#endif
