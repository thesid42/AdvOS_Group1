#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 100
#define MAX_NAME_LEN 3

typedef struct {
    char name[MAX_NAME_LEN];
    int arrival_time;
    int burst_time;
    int priority;
    int start_time;
    int finish_time;
    int remaining_time;
    int started;
} Process;

void generate_processes(Process *procs, int *n, int seed);
int cmp_arrival(const void *a, const void *b);
int cmp_burst(const void *a, const void *b);

#endif
