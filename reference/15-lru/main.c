#include "base.h"
typedef struct Node  {
    char *key;
    struct Node *prev,*next;
} Node;
static void detach(Node *n,Node **head,Node **tail) {
    if(n->prev)n->prev->next=n->next;
    else *head=n->next;
    if(n->next)n->next->prev=n->prev;
    else *tail=n->prev;
}
int main(int argc,char **argv) {
    uint64_t capacity;
    if(argc<2||!uint_arg(argv[1],10000,&capacity)||!capacity)return fail("capacity 1..10000");
    Node *head=NULL,*tail=NULL;
    size_t len=0,hits=0,miss=0;
    bool ok=true;
    for(int i=2;i<argc;i++) {
        Node *n=head;
        while(n&&strcmp(n->key,argv[i]))n=n->next;
        if(n) {
            hits++;
            detach(n,&head,&tail);
        } else {
            miss++;
            n=calloc(1,sizeof *n);
            if(!n) {
                ok=false;
                break;
            }
n->key=strdup(argv[i]);
            if(!n->key) {
                free(n);
                ok=false;
                break;
            }len++;
        }
n->prev=NULL;
        n->next=head;
        if(head)head->prev=n;
        else tail=n;
        head=n;
        if(len>capacity) {
            Node *victim=tail;
            detach(victim,&head,&tail);
            free(victim->key);
            free(victim);
            len--;
        }
    }
if(ok) {
        printf("hit=%zu miss=%zu cache=",hits,miss);
        for(Node *n=head;n;n=n->next)printf("%s%s",n==head?"":",",n->key);
        putchar('\n');
    }
while(head) {
        Node *next=head->next;
        free(head->key);
        free(head);
        head=next;
    }
return ok?0:fail("allocation");
}
