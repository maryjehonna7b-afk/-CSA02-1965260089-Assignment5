/* B2: Safe line input, file error handling and CLI checks */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define NAME_SIZE 32
int main(int argc,char **argv) {
    char name[NAME_SIZE];
    printf("Enter donor name (max %d characters): ",NAME_SIZE-1);
    if (!fgets(name,sizeof name,stdin)) {
        fprintf(stderr,"Error: no input received\n"); return 1;
    }
    size_t len=strlen(name);
    if (len && name[len-1]=='\n') name[len-1]='\0';
    else {
        int ch; while ((ch=getchar())!='\n' && ch!=EOF) {}
        fprintf(stderr,"Warning: name was too long and has been truncated\n");
    }
    printf("Name accepted: %s\n",name);
    if (argc>1) {
        FILE *fp=fopen(argv[1],"r");
        if (!fp) { perror(argv[1]); return 1; }
        fclose(fp);
    }
    if (argc>2) {
        fprintf(stderr,"Usage: %s [optional-data-file]\n",argv[0]); return 1;
    }
    return 0;
}