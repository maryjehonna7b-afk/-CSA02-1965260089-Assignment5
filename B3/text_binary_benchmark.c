/* B3: Compare text and binary file sizes and I/O times.
   Usage: ./benchmark [record-count] ; default 100000 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
typedef struct { int id; char group[4]; int age; char name[32]; } Donor;
static double seconds(clock_t a,clock_t b) { return (double)(b-a)/CLOCKS_PER_SEC; }
int main(int argc,char **argv) {
    long n=100000;
    if (argc==2) { char *e; n=strtol(argv[1],&e,10); if (*e || n<1 || n>1000000) {
        fprintf(stderr,"record-count must be 1..1000000\n"); return 1; } }
    FILE *ft=fopen("donors.txt","w"), *fb=fopen("donors.bin","wb");
    if (!ft || !fb) { perror("fopen"); if(ft)fclose(ft); if(fb)fclose(fb); return 1; }
    Donor d;
    clock_t t0=clock();
    for(long i=0;i<n;++i) {
        d.id=(int)i+1; snprintf(d.group,sizeof d.group,"%s",(i%2)?"A+":"O+");
        d.age=18+(int)(i%50); snprintf(d.name,sizeof d.name,"Donor%ld",i+1);
        if(fprintf(ft,"%d|%s|%d|%s\n",d.id,d.group,d.age,d.name)<0) { perror("fprintf"); return 1; }
    }
    clock_t t1=clock();
    for(long i=0;i<n;++i) {
        d.id=(int)i+1; snprintf(d.group,sizeof d.group,"%s",(i%2)?"A+":"O+");
        d.age=18+(int)(i%50); snprintf(d.name,sizeof d.name,"Donor%ld",i+1);
        if(fwrite(&d,sizeof d,1,fb)!=1) { perror("fwrite"); return 1; }
    }
    clock_t t2=clock();
    fclose(ft); fclose(fb);
    ft=fopen("donors.txt","r"); fb=fopen("donors.bin","rb");
    if(!ft||!fb){perror("reopen");return 1;}
    char line[128]; long read_text=0,read_bin=0;
    while(fgets(line,sizeof line,ft)) ++read_text;
    while(fread(&d,sizeof d,1,fb)==1) ++read_bin;
    clock_t t3=clock(); fclose(ft); fclose(fb);
    struct stat st;
    stat("donors.txt",&st); long text_bytes=(long)st.st_size;
    stat("donors.bin",&st); long bin_bytes=(long)st.st_size;
    printf("records=%ld text_read=%ld binary_read=%ld\n",n,read_text,read_bin);
    printf("text_write_seconds=%.6f binary_write_seconds=%.6f read_seconds=%.6f\n",
           seconds(t0,t1),seconds(t1,t2),seconds(t2,t3));
    printf("text_bytes=%ld binary_bytes=%ld\n",text_bytes,bin_bytes);
    return (read_text==n && read_bin==n)?0:1;
}