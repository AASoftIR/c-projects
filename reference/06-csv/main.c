#include "base.h"
int main(void) {
    enum  {
        START,PLAIN,QUOTED,AFTER
    } state=START;
    size_t rows=0,fields=0,max=0,current=1;
    bool active=false;
    int c;
    while((c=getchar())!=EOF) {
        if(c=='\r')return fail("only LF record endings supported");
        active=true;
        if(state==QUOTED) {
            if(c=='"')state=AFTER;
            continue;
        }
if(state==AFTER && c=='"') {
            state=QUOTED;
            continue;
        }
if(c==',' || c=='\n') {
            if(c==',') {
                current++;
                state=START;
            } else {
                rows++;
                fields+=current;
                if(current>max)max=current;
                current=1;
                state=START;
                active=false;
            }continue;
        }
if(state==AFTER)return fail("data after quote");
        if(c=='"') {
            if(state!=START)return fail("quote in unquoted field");
            state=QUOTED;
        } else state=PLAIN;
    }
if(ferror(stdin)||state==QUOTED)return fail("read failure or unterminated quote");
    if(active) {
        rows++;
        fields+=current;
        if(current>max)max=current;
    }     printf("rows=%zu fields=%zu max_fields=%zu\n",rows,fields,max);
    return 0;
}
