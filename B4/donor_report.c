/* B4: Load donor records into a growable list and print next-week eligibility.
   Input file format: id|group|age|days_since_last_donation */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct { int id; char group[4]; int age; int days_since; } Donor;
typedef struct { Donor *data; size_t size,capacity; } DonorList;
static int add_donor(DonorList *l,const Donor *d) {
    if(l->size==l->capacity) {
        size_t cap=l->capacity?l->capacity*2:64;
        Donor *tmp=realloc(l->data,cap*sizeof *tmp);
        if(!tmp) return 0;
        l->data=tmp; l->capacity=cap;
    }
    l->data[l->size++]=*d; return 1;
}
static int load_file(const char *path,DonorList *l) {
    FILE *fp=fopen(path,"r"); if(!fp){perror(path);return 0;}
    char line[256]; int line_no=0;
    while(fgets(line,sizeof line,fp)) {
        Donor d; ++line_no;
        if(sscanf(line,"%d|%3[^|]|%d|%d",&d.id,d.group,&d.age,&d.days_since)!=4) {
            fprintf(stderr,"Skipping malformed line %d\n",line_no); continue;
        }
        if(!add_donor(l,&d)){fprintf(stderr,"Memory allocation failed\n");fclose(fp);return 0;}
    }
    if(ferror(fp)){perror("read");fclose(fp);return 0;}
    fclose(fp); return 1;
}
int main(int argc,char **argv) {
    const char *path=argc>1?argv[1]:"donors_data.txt";
    DonorList list={NULL,0,0};
    clock_t start=clock();
    if(!load_file(path,&list)){free(list.data);return 1;}
    size_t eligible=0;
    puts("Eligible donors for next-week report (age >= 18 and last donation >= 56 days ago):");
    for(size_t i=0;i<list.size;++i) {
        Donor *d=&list.data[i];
        if(d->age>=18 && d->days_since>=49) {
            printf("%d %s age=%d days_since=%d\n",d->id,d->group,d->age,d->days_since);
            ++eligible;
        }
    }
    double elapsed=(double)(clock()-start)/CLOCKS_PER_SEC;
    printf("loaded=%zu capacity=%zu eligible=%zu allocated_bytes=%zu elapsed_seconds=%.6f\n",
           list.size,list.capacity,eligible,list.capacity*sizeof(Donor),elapsed);
    free(list.data); list.data=NULL;
    return 0;
}