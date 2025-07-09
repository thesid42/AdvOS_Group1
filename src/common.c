#include <stdlib.h>
#include <string.h>
#include "process.h"

int cmp_arrival(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrival_time - p2->arrival_time;
}

int cmp_burst(const void *a, const void *b) {
    Process *p1 = *(Process **)a;
    Process *p2 = *(Process **)b;
    return p1->burst_time - p2->burst_time;
}

void generate_processes(Process *procs, int *n, int seed) {
    srand(seed);
    *n = 0;
    while (*n < MAX_PROCESSES) {
        Process *p = &procs[*n];
        p->name[0] = 'A' + (*n % 26);
        p->name[1] = '\0';
        p->arrival_time = rand() % 100;
        p->burst_time = (rand() % 10) + 1;
        p->priority = (rand() % 4) + 1;
        p->remaining_time = p->burst_time;
        p->started = 0;
        (*n)++;
    }
    qsort(procs, *n, sizeof(Process), cmp_arrival);
}
