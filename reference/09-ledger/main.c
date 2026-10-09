#include "base.h"
int main(int argc,char **argv) {
    if(argc<3)return fail("FILE add TEXT | list");
    if(!strcmp(argv[2],"add") && argc==4) {
        size_t n=strlen(argv[3]);
        if(!n||n>4096||strpbrk(argv[3],"\r\n"))return fail("invalid entry");
        FILE *f=fopen(argv[1],"a");
        if(!f)return fail("open");
        bool ok=fprintf(f,"%s\n",argv[3])>=0 && fflush(f)==0 && fsync(fileno(f))==0;
        if(fclose(f)!=0)ok=false;
        if(!ok)return fail("write");
        puts("added");
        return 0;
    }
if(!strcmp(argv[2],"list") && argc==3) {
        FILE *f=fopen(argv[1],"r");
        if(!f)return fail("open");
        char *line=NULL;
        size_t cap=0,num=0;
        ssize_t n;
        while((n=getline(&line,&cap,f))>=0) {
            printf("%zu:",++num);
            fwrite(line,1,(size_t)n,stdout);
            if(!n||line[n-1]!='\n')putchar('\n');
        }
bool ok=!ferror(f);
        free(line);
        if(fclose(f))ok=false;
        return ok?0:fail("read");
    }
return fail("command");
}
