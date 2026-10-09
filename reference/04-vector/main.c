#include "base.h"
typedef struct  {
    int64_t *data;
    size_t len,cap;
} Vec;
static bool push(Vec *v,int64_t x) {
    if(v->len==v->cap) {
        size_t c=v->cap?v->cap*2:4;
        if(c<v->cap || c>SIZE_MAX/sizeof *v->data)return false;
        void *p=realloc(v->data,c*sizeof *v->data);
        if(!p)return false;
        v->data=p;
        v->cap=c;
    }
v->data[v->len++]=x;
    return true;
}
static int cmp(const void *a,const void *b) {
    int64_t x=*(const int64_t*)a,y=*(const int64_t*)b;
    return (x>y)-(x<y);
}
int main(int argc,char **argv) {
    Vec v= {
        0
    };
    if(argc<2)return fail("integers required");
    for(int i=1;i<argc;i++) {
        int64_t x;
        if(!int_arg(argv[i],&x)||!push(&v,x)) {
            free(v.data);
            return fail("invalid integer or allocation");
        }
    }     qsort(v.data,v.len,sizeof *v.data,cmp);
    for(size_t i=0;i<v.len;i++)printf("%s%" PRId64,i?" ":"",v.data[i]);
    putchar('\n');
    free(v.data);
    return 0;
}
