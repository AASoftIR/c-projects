#include "base.h"
#define BUCKETS 257
typedef struct Entry  {
    char *key;
    size_t count;
    struct Entry *next;
} Entry;
static size_t hash(const char *s) {
    size_t h=5381;
    for(;*s;s++)h=h*33+(unsigned char)*s;
    return h%BUCKETS;
}
static bool add(Entry **table,const char *word) {
    size_t h=hash(word);
    for(Entry *e=table[h];e;e=e->next)if(!strcmp(e->key,word)) {
        if(e->count==SIZE_MAX)return false;
        e->count++;
        return true;
    }
Entry *e=calloc(1,sizeof *e);
    if(!e)return false;
    e->key=strdup(word);
    if(!e->key) {
        free(e);
        return false;
    }
e->count=1;
    e->next=table[h];
    table[h]=e;
    return true;
}
static int cmp(const void *a,const void *b) {
    return strcmp((*(Entry *const*)a)->key,(*(Entry *const*)b)->key);
}
int main(void) {
    Entry *table[BUCKETS]= {
        0
    };
    size_t cap=64,n=0;
    char *word=malloc(cap);
    bool ok=word!=NULL;
    int c=0;
    while(ok && (c=getchar())!=EOF) {
        if(ascii_letter((unsigned char)c)) {
            if(n+1==cap) {
                if(cap>SIZE_MAX/2) {
                    ok=false;
                    break;
                }cap*=2;
                void *p=realloc(word,cap);
                if(!p) {
                    ok=false;
                    break;
                }word=p;
            }word[n++]=(char)ascii_lower((unsigned char)c);
        }         else if(n) {
            word[n]=0;
            ok=add(table,word);
            n=0;
        }
    }
if(ok && n) {
        word[n]=0;
        ok=add(table,word);
    }
if(ferror(stdin))ok=false;
    free(word);
    size_t total=0;
    for(size_t b=0;b<BUCKETS;b++)for(Entry *e=table[b];e;e=e->next)total++;
    Entry **sorted=NULL;
    if(total) {
        if(total>SIZE_MAX/sizeof *sorted)ok=false;
        else sorted=malloc(total*sizeof *sorted);
        if(!sorted)ok=false;
    }
if(ok && total) {
        size_t j=0;
        for(size_t b=0;b<BUCKETS;b++)for(Entry *e=table[b];e;e=e->next)sorted[j++]=e;
        qsort(sorted,total,sizeof *sorted,cmp);
        for(size_t i=0;i<total;i++)printf("%s=%zu\n",sorted[i]->key,sorted[i]->count);
    }
free(sorted);
    for(size_t b=0;b<BUCKETS;b++) {
        Entry *e=table[b];
        while(e) {
            Entry *next=e->next;
            free(e->key);
            free(e);
            e=next;
        }
    }
return ok?0:fail("read or allocation failure");
}
