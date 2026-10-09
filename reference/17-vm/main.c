#include "base.h"
static bool add64(int64_t a,int64_t b,int64_t *r) {
    if((b>0&&a>INT64_MAX-b)||(b<0&&a<INT64_MIN-b))return false;
    *r=a+b;
    return true;
}
static bool sub64(int64_t a,int64_t b,int64_t *r) {
    if((b<0&&a>INT64_MAX+b)||(b>0&&a<INT64_MIN+b))return false;
    *r=a-b;
    return true;
}
static bool mul64(int64_t a,int64_t b,int64_t *r) {
    if(a>0) {
        if((b>0&&a>INT64_MAX/b)||(b<0&&b<INT64_MIN/a))return false;
    } else if(a<0) {
        if((b>0&&a<INT64_MIN/b)||(b<0&&a<INT64_MAX/b))return false;
    }
*r=a*b;
    return true;
}
int main(void) {
    int64_t stack[1024];
    size_t top=0,cap=0;
    char *line=NULL;
    ssize_t n;
    bool ok=true;
    while(ok&&(n=getline(&line,&cap,stdin))>=0) {
        (void)n;
        char *save=NULL,*op=strtok_r(line," \t\r\n",&save);
        if(!op)continue;
        char *arg=strtok_r(NULL," \t\r\n",&save),*extra=strtok_r(NULL," \t\r\n",&save);
        if(!strcmp(op,"PUSH")) {
            int64_t v;
            if(!arg||extra||top==1024||!int_arg(arg,&v))ok=false;
            else stack[top++]=v;
            continue;
        }
if(arg) {
            ok=false;
            break;
        }
if(!strcmp(op,"PRINT")) {
            if(!top)ok=false;
            else printf("%" PRId64 "\n",stack[top-1]);
            continue;
        }
if(top<2) {
            ok=false;
            break;
        }
int64_t b=stack[--top],a=stack[top-1],v=0;
        if(!strcmp(op,"ADD"))ok=add64(a,b,&v);
        else if(!strcmp(op,"SUB"))ok=sub64(a,b,&v);
        else if(!strcmp(op,"MUL"))ok=mul64(a,b,&v);
        else if(!strcmp(op,"DIV")) {
            ok=b!=0&&!(a==INT64_MIN&&b==-1);
            if(ok)v=a/b;
        } else ok=false;
        if(ok)stack[top-1]=v;
    }
if(ferror(stdin))ok=false;
    free(line);
    return ok?0:fail("invalid bytecode or arithmetic");
}
