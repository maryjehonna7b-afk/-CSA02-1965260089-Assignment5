/* A4: Growable donor array with safe realloc handling */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { int id; char group[4]; } Donor;
typedef struct { Donor *data; size_t size,capacity; } DonorList;

int add_donor(DonorList *list,int id,const char *group) {
    if (!list || !group) return 0;
    if (list->size==list->capacity) {
        size_t new_capacity=list->capacity==0 ? 2 : list->capacity*2;
        Donor *temp=realloc(list->data,new_capacity*sizeof *temp);
        if (!temp) return 0;
        list->data=temp;
        list->capacity=new_capacity;
    }
    list->data[list->size].id=id;
    snprintf(list->data[list->size].group,sizeof list->data[list->size].group,"%s",group);
    ++list->size;
    return 1;
}
int main(void) {
    DonorList list={NULL,0,0};
    const char *groups[]={"O+","A+","B-","AB+","O-"};
    for (size_t i=0;i<5;++i) {
        if (!add_donor(&list,7001+(int)i,groups[i])) {
            perror("realloc"); free(list.data); return 1;
        }
        printf("added %d (%s): size=%zu capacity=%zu\n",
               list.data[i].id,list.data[i].group,list.size,list.capacity);
    }
    free(list.data);
    list.data=NULL; list.size=list.capacity=0;
    return 0;
}