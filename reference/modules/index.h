#ifndef COURSE_INDEX_H
#define COURSE_INDEX_H
#include "base.h"
#define INDEX_BUCKETS 1021
typedef struct Posting  {
    size_t doc;
    struct Posting *next;
} Posting;
typedef struct Term  {
    char *word;
    Posting *head,*tail;
    struct Term *next;
} Term;
typedef struct  {
    Term *buckets[INDEX_BUCKETS];
    size_t docs;
} Index;
static inline size_t word_hash(const char *s) {
    size_t h=5381;
    for(;*s;s++)h=h*33+(unsigned char)*s;
    return h%INDEX_BUCKETS;
}
static inline Term *index_term(Index *x,const char *word) {
    for(Term *t=x->buckets[word_hash(word)];t;t=t->next)if(!strcmp(t->word,word))return t;
    return NULL;
}
static inline bool index_add(Index *x,const char *word,size_t doc) {
    size_t h=word_hash(word);
    Term *t=index_term(x,word);
    if(!t) {
        t=calloc(1,sizeof *t);
        if(!t)return false;
        t->word=strdup(word);
        if(!t->word) {
            free(t);
            return false;
        }
t->next=x->buckets[h];
        x->buckets[h]=t;
    }
if(t->tail&&t->tail->doc==doc)return true;
    Posting *p=calloc(1,sizeof *p);
    if(!p)return false;
    p->doc=doc;
    if(t->tail)t->tail->next=p;
    else t->head=p;
    t->tail=p;
    return true;
}
static inline void index_free(Index *x) {
    for(size_t b=0;b<INDEX_BUCKETS;b++) {
        Term *t=x->buckets[b];
        while(t) {
            Term *next=t->next;
            Posting *p=t->head;
            while(p) {
                Posting *pn=p->next;
                free(p);
                p=pn;
            }
free(t->word);
            free(t);
            t=next;
        }
x->buckets[b]=NULL;
    }
x->docs=0;
}
static inline bool index_load(Index *x,const char *path) {
    FILE *f=fopen(path,"r");
    if(!f)return false;
    char *line=NULL;
    size_t cap=0;
    bool ok=true;
    while(ok&&getline(&line,&cap,f)>=0) {
        if(x->docs==SIZE_MAX) {
            ok=false;
            break;
        }
size_t doc=++x->docs;
        char *p=line;
        while(*p) {
            while(*p&&!ascii_letter((unsigned char)*p))p++;
            if(!*p)break;
            char *start=p;
            while(ascii_letter((unsigned char)*p)) {
                *p=(char)ascii_lower((unsigned char)*p);
                p++;
            }char end=*p;
            *p=0;
            if(!index_add(x,start,doc)) {
                ok=false;
                break;
            }
if(!end)break;
            p++;
        }
    }
if(ferror(f))ok=false;
    free(line);
    if(fclose(f))ok=false;
    return ok;
}
static inline bool query_word(const char *s,char *out,size_t cap) {
    size_t n=strlen(s);
    if(!n||n>=cap)return false;
    for(size_t i=0;i<n;i++) {
        if(!ascii_letter((unsigned char)s[i]))return false;
        out[i]=(char)ascii_lower((unsigned char)s[i]);
    }out[n]=0;
    return true;
}
#endif
