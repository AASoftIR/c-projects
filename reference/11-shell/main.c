#include "base.h"
#include <sys/wait.h>
#define MAX_CMDS 32
#define MAX_ARGS 64
static int run(char *text) {
    char *cmds[MAX_CMDS][MAX_ARGS];
    size_t nc=0,na=0;
    bool lastpipe=false;
    char *save=NULL;
    for(char *t=strtok_r(text," \t\r\n",&save);t;t=strtok_r(NULL," \t\r\n",&save)) {
        if(!strcmp(t,"|")) {
            if(!na||nc+1>=MAX_CMDS)return fail("invalid pipeline");
            cmds[nc][na]=NULL;
            nc++;
            na=0;
            lastpipe=true;
        } else {
            if(na+1>=MAX_ARGS)return fail("too many arguments");
            cmds[nc][na++]=t;
            lastpipe=false;
        }
    }
if(!na)return lastpipe?fail("trailing pipe"):0;
    cmds[nc][na]=NULL;
    nc++;
    if(nc==1&&!strcmp(cmds[0][0],"cd")) {
        if(na!=2)return fail("cd PATH");
        return chdir(cmds[0][1])?fail("chdir"):0;
    }
if(nc==1&&!strcmp(cmds[0][0],"exit"))return na==1?0:fail("exit takes no arguments");
    pid_t children[MAX_CMDS];
    size_t started=0;
    int previous=-1,result=0;
    for(size_t i=0;i<nc;i++) {
        int fd[2]= {
            -1,-1
        };
        if(i+1<nc&&pipe(fd)<0) {
            result=2;
            break;
        }pid_t child=fork();
        if(child<0) {
            if(fd[0]>=0) {
                close(fd[0]);
                close(fd[1]);
            }result=2;
            break;
        }
if(child==0) {
            if(previous>=0&&dup2(previous,STDIN_FILENO)<0)_exit(126);
            if(fd[1]>=0&&dup2(fd[1],STDOUT_FILENO)<0)_exit(126);
            if(previous>=0)close(previous);
            if(fd[0]>=0)close(fd[0]);
            if(fd[1]>=0)close(fd[1]);
            execvp(cmds[i][0],cmds[i]);
            perror("execvp");
            _exit(127);
        }         children[started++]=child;
        if(previous>=0)close(previous);
        if(fd[1]>=0)close(fd[1]);
        previous=fd[0];
    }
if(previous>=0)close(previous);
    for(size_t i=0;i<started;i++) {
        int status;
        pid_t w;
        do {
            w=waitpid(children[i],&status,0);
        }
while(w<0&&errno==EINTR);
        if(w<0) {
            result=2;
            continue;
        }
if(!result&&i+1==nc)result=WIFEXITED(status)?WEXITSTATUS(status):128+WTERMSIG(status);
    }
return result;
}
int main(int argc,char **argv) {
    if(argc==3&&!strcmp(argv[1],"-c")) {
        char *s=strdup(argv[2]);
        if(!s)return fail("allocation");
        int r=run(s);
        free(s);
        return r;
    }
if(argc!=1)return fail("[-c COMMAND]");
    char *s=NULL;
    size_t cap=0;
    int r=0;
    while(getline(&s,&cap,stdin)>=0) {
        if(!strcmp(s,"exit\n")||!strcmp(s,"exit"))break;
        r=run(s);
    }
free(s);
    return ferror(stdin)?fail("read"):r;
}
