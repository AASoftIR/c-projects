#include "base.h"
#include <pthread.h>
typedef struct  {
    uint64_t start,end,sum;
} Job;
static void *work(void *arg) {
    Job *j=arg;
    j->sum=0;
    for(uint64_t n=j->start;n<j->end;n++)j->sum+=n;
    return NULL;
}
int main(int argc,char **argv) {
    uint64_t threads,count;
    if(argc!=3||!uint_arg(argv[1],64,&threads)||!threads||!uint_arg(argv[2],10000000,&count))return fail("THREADS 1..64 COUNT 0..10000000");
    pthread_t ids[64];
    Job jobs[64];
    size_t made=0;
    bool ok=true;
    for(size_t i=0;i<(size_t)threads;i++) {
        jobs[i]=(Job) {
            1+count*i/threads,1+count*(i+1)/threads,0
        };
        if(pthread_create(&ids[i],NULL,work,&jobs[i])) {
            ok=false;
            break;
        }made++;
    }     uint64_t total=0;
    for(size_t i=0;i<made;i++) {
        if(pthread_join(ids[i],NULL))ok=false;
        else total+=jobs[i].sum;
    }
if(!ok)return fail("thread failure");
    printf("sum=%" PRIu64 "\n",total);
    return 0;
}
