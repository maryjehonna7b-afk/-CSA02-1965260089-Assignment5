/* A3: Pointer operations, lowest-stock pointer and CLI validation */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int *lowest_stock(int *stock, size_t count) {
    if (!stock || count==0) return NULL;
    size_t min_i=0;
    for (size_t i=1;i<count;++i) if (stock[i]<stock[min_i]) min_i=i;
    return &stock[min_i];
}
static int valid_group(const char *s) {
    const char *groups[]={"A+","A-","B+","B-","AB+","AB-","O+","O-"};
    for (size_t i=0;i<8;++i) if (strcmp(s,groups[i])==0) return 1;
    return 0;
}
int main(int argc,char **argv) {
    int stock[8]={20,15,30,10,25,12,40,18};
    int *p=stock;
    printf("p+3 points to stock index 3; *(p+3)=%d; p[3]=%d; *p++=%d\n",
           *(p+3),p[3],*p++);
    if (argc<3) {
        fprintf(stderr,"Usage: %s find <blood-group> | issue <blood-group> <units>\n",argv[0]);
        return 1;
    }
    if (!valid_group(argv[2])) { fprintf(stderr,"Error: invalid blood group\n"); return 1; }
    if (strcmp(argv[1],"find")==0 && argc==3) {
        int *lowest=lowest_stock(stock,8);
        printf("Lowest stock: %d units (array element index %ld)\n",
               *lowest,(long)(lowest-stock));
        return 0;
    }
    if (strcmp(argv[1],"issue")==0 && argc==4) {
        char *end=NULL; long units=strtol(argv[3],&end,10);
        if (!end || *end!='\0' || units<=0 || units>100000) {
            fprintf(stderr,"Error: units must be a positive integer\n"); return 1;
        }
        printf("Validated issue request: %s %ld units\n",argv[2],units);
        return 0;
    }
    fprintf(stderr,"Error: invalid command or argument count\n");
    return 1;
}