/* A1: Modular blood-bank design demonstration */
#include <stdio.h>
#include <string.h>
#include <stddef.h>

typedef struct {
    int id;
    char name[64];
    char group[4];
    int age;
    char phone[16];
    char last_donation[11];
} Donor;

typedef struct {
    char group[4];
    int units;
} Stock;

typedef struct {
    size_t eligible_count;
    int youngest_id;
} EligibilitySummary;

int register_donor(Donor donors[], size_t *count, size_t capacity, const Donor *new_donor);
int check_eligibility(const Donor *donor, int minimum_age);
size_t find_donors(const Donor donors[], size_t count, const char *group, int minimum_age,
                   int *youngest_id);
int update_stock(Stock stock[], size_t count, const char *group, int delta);
EligibilitySummary summarize_eligible(const Donor donors[], size_t count,
                                      const char *group, int minimum_age);

int register_donor(Donor donors[], size_t *count, size_t capacity, const Donor *new_donor) {
    if (!donors || !count || !new_donor || *count >= capacity) return 0;
    donors[*count] = *new_donor;
    ++(*count);
    return 1;
}
int check_eligibility(const Donor *donor, int minimum_age) {
    return donor != NULL && donor->age >= minimum_age;
}
size_t find_donors(const Donor donors[], size_t count, const char *group, int minimum_age,
                   int *youngest_id) {
    size_t found = 0;
    int youngest_age = 1000000;
    if (!donors || !group) return 0;
    for (size_t i=0; i<count; ++i) {
        if (strcmp(donors[i].group, group)==0 && check_eligibility(&donors[i], minimum_age)) {
            ++found;
            if (donors[i].age < youngest_age) {
                youngest_age = donors[i].age;
                if (youngest_id) *youngest_id = donors[i].id;
            }
        }
    }
    return found;
}
int update_stock(Stock stock[], size_t count, const char *group, int delta) {
    if (!stock || !group) return 0;
    for (size_t i=0; i<count; ++i) if (strcmp(stock[i].group,group)==0) {
        if (stock[i].units + delta < 0) return 0;
        stock[i].units += delta;
        return 1;
    }
    return 0;
}
EligibilitySummary summarize_eligible(const Donor donors[], size_t count,
                                      const char *group, int minimum_age) {
    EligibilitySummary result = {0, -1};
    result.eligible_count = find_donors(donors,count,group,minimum_age,&result.youngest_id);
    return result;
}
int main(void) {
    Donor donors[3] = {
        {101,"Anu","O+",22,"9000000001","2025-01-01"},
        {102,"Bala","O+",25,"9000000002","2025-02-01"},
        {103,"Ciya","A-",19,"9000000003","2025-03-01"}
    };
    EligibilitySummary s = summarize_eligible(donors,3,"O+",18);
    printf("Eligible O+ donors: %zu; youngest eligible donor ID: %d\n",
           s.eligible_count,s.youngest_id);
    return 0;
}