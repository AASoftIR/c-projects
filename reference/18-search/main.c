#include "index.h"
int main(int argc,char **argv) {
    if(argc!=3)return fail("CORPUS WORD");
    char word[256];
    if(!query_word(argv[2],word,sizeof word))return fail("single ASCII word required");
    Index x= {
        0
    };
    if(!index_load(&x,argv[1])) {
        index_free(&x);
        return fail("corpus load");
    }
Term *t=index_term(&x,word);
    if(t)for(Posting *p=t->head;p;p=p->next)printf("%zu\n",p->doc);
    bool found = t != NULL;
    index_free(&x);
    return found ? 0 : 1;
}
