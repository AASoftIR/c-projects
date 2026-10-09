#include "base.h"
typedef struct  {
    const char *s;
    bool error;
    unsigned depth;
} Parser;
static void ws(Parser *p) {
    while(isspace((unsigned char)*p->s))p->s++;
}
static double expr(Parser *p);
static double primary(Parser *p) {
    ws(p);
    if(++p->depth>128) {
        p->error=true;
        p->depth--;
        return 0;
    }double v=0;
    if(*p->s=='(') {
        p->s++;
        v=expr(p);
        ws(p);
        if(*p->s!=')')p->error=true;
        else p->s++;
    }     else  {
        char *end;
        errno=0;
        v=strtod(p->s,&end);
        if(end==p->s||errno||!isfinite(v))p->error=true;
        p->s=end;
    }
p->depth--;
    return v;
}
static double unary(Parser *p) {
    ws(p);
    int sign=1;
    while(*p->s=='+'||*p->s=='-') {
        if(*p->s++=='-')sign=-sign;
        ws(p);
    }
return sign*primary(p);
}
static double term(Parser *p) {
    double v=unary(p);
    ws(p);
    while(!p->error && (*p->s=='*'||*p->s=='/')) {
        char op=*p->s++;
        double rhs=unary(p);
        if(op=='/'&&rhs==0) {
            p->error=true;
            return 0;
        }v=op=='*'?v*rhs:v/rhs;
        if(!isfinite(v))p->error=true;
        ws(p);
    }
return v;
}
static double expr(Parser *p) {
    double v=term(p);
    ws(p);
    while(!p->error&&(*p->s=='+'||*p->s=='-')) {
        char op=*p->s++;
        double rhs=term(p);
        v=op=='+'?v+rhs:v-rhs;
        if(!isfinite(v))p->error=true;
        ws(p);
    }
return v;
}
int main(int argc,char **argv) {
    if(argc!=2)return fail("EXPR");
    Parser p= {
        argv[1],false,0
    };
    double v=expr(&p);
    ws(&p);
    if(p.error||*p.s)return fail("invalid expression");
    printf("%.10g\n",v);
    return 0;
}
