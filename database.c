#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "database.h"

// holds a single row for the file
typedef struct {
    char ts[30];
    char ds[100];
    char al[100];
    int n;
    double t;
} Entry;

// sorting rules for the db
int compareEntries(const void* a, const void* b) {
    Entry* e1 = (Entry*)a;
    Entry* e2 = (Entry*)b;
    
    // check dataset first
    int res = strcmp(e1->ds, e2->ds);
    if (res != 0) return res;
    
    // then algorithm
    res = strcmp(e1->al, e2->al);
    if (res != 0) return res;
    
    // then when it happened
    return strcmp(e1->ts, e2->ts);
}

// saves things to csv and sorts them
void save_to_db(const char* test_name, const char* algo_name, int n, double time_taken) {
    const char* filename = "results.csv";
    Entry entries[3000];
    int count = 0;

    // read old file
    FILE* f = fopen(filename, "r");
    if (f != NULL) {
        char header[200];
        if (fgets(header, 200, f)) {
            while (count < 2999) {
                if (fscanf(f, " \"%29[^\"]\",\"%99[^\"]\",\"%99[^\"]\",%d,%lf", 
                    entries[count].ts, entries[count].ds, entries[count].al, 
                    &entries[count].n, &entries[count].t) == 5) {
                    count++;
                } else {
                    break;
                }
            }
        }
        fclose(f);
    }

    // get current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(entries[count].ts, 30, "%Y-%m-%d %H:%M:%S", tm_info); 
    
    // copy strings to struct
    strncpy(entries[count].ds, test_name, 99);
    strncpy(entries[count].al, algo_name, 99);
    entries[count].n = n;
    entries[count].t = time_taken;
    count++;

    // sort all of it
    qsort(entries, count, sizeof(Entry), compareEntries);

    // write back to file
    f = fopen(filename, "w");
    if (f == NULL) {
        printf("database error\n");
        return;
    }

    fprintf(f, "Timestamp,Dataset,Algorithm,N,TimeSeconds\n");
    for (int i = 0; i < count; i++) {
        fprintf(f, "\"%s\",\"%s\",\"%s\",%d,%f\n", 
            entries[i].ts, entries[i].ds, entries[i].al, 
            entries[i].n, entries[i].t);
    }
    
    fclose(f);
    printf("   [db] saved\n");
}
