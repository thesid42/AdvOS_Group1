#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 100
#define MAX_NAME_LEN 3
#define MAX_QUANTA 1000 

extern char time_chart[MAX_QUANTA];
extern int chart_index;

void add_to_time_chart(char pid);
void print_time_chart();
void reset_time_chart();

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

typedef struct {
    double avg_turnaround;
    double avg_waiting;
    double avg_response;
    double throughput;
} AlgorithmStats;

void generate_processes(Process *procs, int *n, int seed);
void print_processes(Process *procs, int n);
void print_algorithm_summary(const char* algorithm_name, AlgorithmStats stats[], int runs);
int cmp_arrival(const void *a, const void *b);
int cmp_burst(const void *a, const void *b);

#endif
