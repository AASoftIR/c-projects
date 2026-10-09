#include "http.h"
int main(int argc,char **argv) {
    uint64_t port;
    if(argc!=2||!uint_arg(argv[1],65535,&port))return fail("PORT");
    signal(SIGPIPE,SIG_IGN);
    int server=listen_loopback((unsigned)port);
    if(server<0)return fail("listen");
    for(;;) {
        int fd=accept(server,NULL,NULL);
        if(fd<0) {
            if(errno==EINTR)continue;
            close(server);
            return fail("accept");
        }
Request r;
        if(!receive_request(fd,&r))response(fd,400,"{\"error\":\"bad request\"}\n");
        else if(!strcmp(r.target,"/health"))response(fd,200,"{\"status\":\"ok\"}\n");
        else response(fd,404,"{\"error\":\"not found\"}\n");
        close(fd);
    }
}
