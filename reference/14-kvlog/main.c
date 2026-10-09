#include "base.h"
#include <sys/types.h>
#include <sys/stat.h>
typedef struct Item  {
    char *key,*value;
    struct Item *next;
} Item;
static bool valid(const char *s,size_t max) {
    size_t n=strlen(s);
    if(!n||n>max)return false;
    for(size_t i=0;i<n;i++)if((unsigned char)s[i]<32||(unsigned char)s[i]>126)return false;
    return true;
}
static Item *find(Item *head,const char *key) {
    for(;head;head=head->next)if(!strcmp(head->key,key))return head;
    return NULL;
}
static bool assign(Item **head,const char *key,const char *value) {
    Item *e=find(*head,key);
    char *copy=value?strdup(value):NULL;
    if(value&&!copy)return false;
    if(!e) {
        e=calloc(1,sizeof *e);
        if(!e) {
            free(copy);
            return false;
        }
e->key=strdup(key);
        if(!e->key) {
            free(copy);
            free(e);
            return false;
        }
e->next=*head;
        *head=e;
    }
free(e->value);
    e->value=copy;
    return true;
}
int main(int argc,char **argv) {
    if(argc<4)return fail("FILE put KEY VALUE | get KEY | del KEY");
    bool put=!strcmp(argv[2],"put"),del=!strcmp(argv[2],"del"),get=!strcmp(argv[2],"get");
    if((put?argc!=5:argc!=4)||(!put&&!del&&!get)||!valid(argv[3],255)||(put&&!valid(argv[4],1023)))return fail("invalid command/key/value");
    FILE *f=fopen(argv[1],get?"r":"a+");
    if(!f)return get&&errno==ENOENT?1:fail("open");
    struct stat st;
    if (fstat(fileno(f), &st) || !S_ISREG(st.st_mode)) { fclose(f); return fail("regular file required"); }
    rewind(f);
    Item *head=NULL;
    char *line=NULL;
    size_t cap=0;
    ssize_t n;
    off_t good=0;
    bool torn=false,ok=true;
    while((n=getline(&line,&cap,f))>=0) {
        if(!n||line[n-1]!='\n') {
            torn=true;
            break;
        }line[n-1]=0;
        char *key=NULL,*value=NULL;
        if(n>=3 && line[1]=='\t' && line[0]=='P') {
            key=line+2;
            value=strchr(key,'\t');
            if(value)*value++=0;
            if(!value||!valid(key,255)||!valid(value,1023)) {
                ok=false;
                break;
            }
        }         else if(n>=3 && line[1]=='\t' && line[0]=='D') {
            key=line+2;
            if(!valid(key,255)) {
                ok=false;
                break;
            }
        }         else {
            ok=false;
            break;
        }
if(!assign(&head,key,value)) {
            ok=false;
            break;
        }good=ftello(f);
        if(good<0) {
            ok=false;
            break;
        }
    }
if(ferror(f))ok=false;
    free(line);
    int result=0;
    if(ok&&get) {
        Item *e=find(head,argv[3]);
        if(e&&e->value)puts(e->value);
        else result=1;
    }     else if(ok) {
        if(torn&&ftruncate(fileno(f),good))ok=false;
        if(ok&&fseeko(f,0,SEEK_END))ok=false;
        if(ok) {
            int w=put?fprintf(f,"P\t%s\t%s\n",argv[3],argv[4]):fprintf(f,"D\t%s\n",argv[3]);
            ok=w>=0&&fflush(f)==0&&fsync(fileno(f))==0;
        }
if(ok)puts("ok");
    }
if(fclose(f))ok=false;
    while(head) {
        Item *next=head->next;
        free(head->key);
        free(head->value);
        free(head);
        head=next;
    }
return ok?result:fail("corrupt log or I/O failure");
}
