#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "process.h"

char time_chart[MAX_QUANTA];
int chart_index = 0;

void add_to_time_chart(char pid) {
    if (chart_index < MAX_QUANTA) {
        time_chart[chart_index++] = pid;
    }
}

void print_time_chart() {
    printf("Time Chart: ");
    for (int i = 0; i < chart_index; i++) {
        printf("%c", time_chart[i]);
    }
    printf("\n");
}

void reset_time_chart() {
    chart_index = 0;
}

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

int validate_idle_time(Process *procs, int n) {
    int time = 0;
    int idle_count = 0;
    
    for (int i = 0; i < n; i++) {
        if (procs[i].arrival_time > time) {
            int idle_time = procs[i].arrival_time - time;
            if (idle_time > 2) {
                return 0; // Too much idle time
            }
            idle_count += idle_time;
        }
        time = (procs[i].arrival_time > time) ? procs[i].arrival_time : time;
        time += procs[i].burst_time;
    }
    
    return 1; // Valid workload
}

void generate_processes(Process *procs, int *n, int seed) {
    srand(seed);
    
    do {
        *n = 10 + (rand() % 6); // Generate 10-15 processes initially
        
        for (int i = 0; i < *n; i++) {
            Process *p = &procs[i];
            p->name[0] = 'A' + (i % 26);
            p->name[1] = '\0';
            p->arrival_time = rand() % 100;
            p->burst_time = (rand() % 10) + 1;
            p->priority = (rand() % 4) + 1;
            p->remaining_time = p->burst_time;
            p->started = 0;
            p->start_time = -1;
            p->finish_time = -1;
        }
        
        qsort(procs, *n, sizeof(Process), cmp_arrival);
        
        // Remove processes that would start after quantum 99
        int valid_count = 0;
        for (int i = 0; i < *n; i++) {
            if (procs[i].arrival_time <= 99) {
                if (i != valid_count) {
                    procs[valid_count] = procs[i];
                }
                valid_count++;
            }
        }
        *n = valid_count;
        
    } while (*n < 10 || !validate_idle_time(procs, *n));
}

void print_processes(Process *procs, int n) {
    printf("Generated Processes:\n");
    printf("Name\tArrival\tBurst\tPriority\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\n", 
               procs[i].name, procs[i].arrival_time, 
               procs[i].burst_time, procs[i].priority);
    }
    printf("\n");
}


void print_algorithm_summary(const char* algorithm_name, AlgorithmStats stats[], int runs) {
    double sum_turnaround = 0, sum_waiting = 0, sum_response = 0, sum_throughput = 0;
    
    for (int i = 0; i < runs; i++) {
        sum_turnaround += stats[i].avg_turnaround;
        sum_waiting += stats[i].avg_waiting;
        sum_response += stats[i].avg_response;
        sum_throughput += stats[i].throughput;
    }
    
    printf("\n=== %s Algorithm Summary (Average of %d runs) ===\n", algorithm_name, runs);
    printf("Average Turnaround Time: %.2f\n", sum_turnaround / runs);
    printf("Average Waiting Time   : %.2f\n", sum_waiting / runs);
    printf("Average Response Time  : %.2f\n", sum_response / runs);
    printf("Average Throughput     : %.2f\n", sum_throughput / runs);
    printf("================================================\n");
}
