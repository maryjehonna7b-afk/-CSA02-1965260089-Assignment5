/* A5: Text donor storage and fixed-size binary stock update */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { int group_code; int units; } Stock;
typedef struct { int id; char name[40]; char group[4]; int age; } Donor;

int save_donor_text(const char *path,const Donor *d) {
    FILE *fp=fopen(path,"a");
    if (!fp) { perror(path); return 0; }
    int ok=fprintf(fp,"%d|%s|%s|%d\n",d->id,d->name,d->group,d->age)>=0;
    if (fclose(fp)!=0) ok=0;
    return ok;
}
int update_stock_binary(FILE *fp,int index,int units) {
    Stock s;
    if (!fp || index<0) return 0;
    if (fseek(fp,(long)index*(long)sizeof s,SEEK_SET)!=0) return 0;
    if (fread(&s,sizeof s,1,fp)!=1) return 0;
    s.units=units;
    if (fseek(fp,-(long)sizeof s,SEEK_CUR)!=0) return 0;
    return fwrite(&s,sizeof s,1,fp)==1 && fflush(fp)==0;
}
int main(void) {
    Donor d={501,"SampleDonor","O+",24};
    if (!save_donor_text("donors.txt",&d)) return 1;
    FILE *fp=tmpfile();
    if (!fp) { perror("tmpfile"); return 1; }
    Stock initial[3]={{1,40},{2,25},{3,12}};
    if (fwrite(initial,sizeof *initial,3,fp)!=3) { perror("fwrite"); fclose(fp); return 1; }
    printf("update index 2: %s\n",update_stock_binary(fp,2,20)?"done":"failed");
    printf("update index 8: %s\n",update_stock_binary(fp,8,20)?"done":"failed");
    fclose(fp);
    return 0;
}