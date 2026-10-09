#include "http.h"
#include "index.h"
static int hex_digit(unsigned char c) {
    if(c>='0'&&c<='9')return c-'0';
    c=ascii_lower(c);
    if(c>='a'&&c<='f')return c-'a'+10;
    return -1;
}
static bool decode(const char *s,char *out,size_t cap) {
    size_t n=0;
    while(*s) {
        unsigned char c=(unsigned char)*s++;
        if(c=='%') {
            if(!s[0]||!s[1])return false;
            int a=hex_digit((unsigned char)s[0]),b=hex_digit((unsigned char)s[1]);
            if(a<0||b<0)return false;
            c=(unsigned char)(a*16+b);
            s+=2;
        } else if(c=='+')c=' ';
        if(!c||n+1>=cap)return false;
        out[n++]=(char)c;
    }out[n]=0;
    return true;
}
static void serve(int fd,Request *r,Index *x) {
    if(!strcmp(r->target,"/health")) {
        response(fd,200,"{\"status\":\"ok\"}\n");
        return;
    }
if(strncmp(r->target,"/search?q=",10)) {
        response(fd,!strncmp(r->target,"/search",7)?400:404,"{\"error\":\"route or query\"}\n");
        return;
    }     char decoded[256],word[256];
    if(!decode(r->target+10,decoded,sizeof decoded)||!query_word(decoded,word,sizeof word)) {
        response(fd,400,"{\"error\":\"single ASCII word required\"}\n");
        return;
    }
Term *t=index_term(x,word);
    size_t count=0;
    if(t)for(Posting *p=t->head;p;p=p->next)count++;
    if(count>(SIZE_MAX-64)/32) {
        response(fd,500,"{\"error\":\"size\"}\n");
        return;
    }
size_t cap=64+count*32;
    char *body=malloc(cap);
    if(!body) {
        response(fd,500,"{\"error\":\"allocation\"}\n");
        return;
    }
size_t n=(size_t)snprintf(body,cap,"{\"documents\":[");
    if(t)for(Posting *p=t->head;p;p=p->next)n+=(size_t)snprintf(body+n,cap-n,"%s%zu",p==t->head?"":",",p->doc);
    (void)snprintf(body+n,cap-n,"]}\n");
    response(fd,200,body);
    free(body);
}
int main(int argc,char **argv) {
    uint64_t port;
    if(argc!=3||!uint_arg(argv[1],65535,&port))return fail("PORT CORPUS");
    Index x= {
        0
    };
    if(!index_load(&x,argv[2])) {
        index_free(&x);
        return fail("corpus");
    }signal(SIGPIPE,SIG_IGN);
    int server=listen_loopback((unsigned)port);
    if(server<0) {
        index_free(&x);
        return fail("listen");
    }
for(;;) {
        int fd=accept(server,NULL,NULL);
        if(fd<0) {
            if(errno==EINTR)continue;
            close(server);
            index_free(&x);
            return fail("accept");
        }
Request r;
        if(!receive_request(fd,&r))response(fd,400,"{\"error\":\"bad request\"}\n");
        else serve(fd,&r,&x);
        close(fd);
    }
}
