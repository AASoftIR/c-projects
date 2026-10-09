#include "base.h"
static bool has(const char *line,const char *pat,bool fold) {
    size_t n=strlen(pat);
    if(!n)return true;
    for(size_t i=0;line[i];i++) {
        size_t j=0;
        while(j<n && line[i+j]) {
            unsigned char a=(unsigned char)line[i+j],b=(unsigned char)pat[j];
            if(fold) {
                a=ascii_lower(a);
                b=ascii_lower(b);
            }
if(a!=b)break;
            j++;
        }
if(j==n)return true;
    }
return false;
}
int main(int argc,char **argv) {
    if(argc<2 || argc>3 || (argc==3 && strcmp(argv[2],"-i")))return fail("PATTERN [-i]");
    char *line=NULL;
    size_t cap=0,index=0;
    ssize_t n;
    bool found=false;
    while((n=getline(&line,&cap,stdin))>=0) {
        index++;
        if(has(line,argv[1],argc==3)) {
            printf("%zu:",index);
            fwrite(line,1,(size_t)n,stdout);
            if(n==0 || line[n-1]!='\n')putchar('\n');
            found=true;
        }
    }
free(line);
    if(ferror(stdin))return fail("read failed");
    return found?0:1;
}
