#include <stdio.h>
#include <stdlib.h>
#include "process.h"

extern int cmp_burst(const void *, const void *);

void run_sjf(Process *procs, int n) {
    int time = 0, completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    reset_time_chart();

    while (completed < n) {
        Process *ready[MAX_PROCESSES];
        int ready_count = 0;

        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && procs[i].arrival_time <= time)
                ready[ready_count++] = &procs[i];
        }

        if (ready_count == 0) {
            time++;
            continue;
        }

        qsort(ready, ready_count, sizeof(Process *), cmp_burst);
        Process *curr = ready[0];

        curr->start_time = time;
        for (int q = 0; q < curr->burst_time; q++) {
            add_to_time_chart(curr->name[0]);
        } 
        curr->finish_time = time + curr->burst_time;
        time += curr->burst_time;

        int turnaround = curr->finish_time - curr->arrival_time;
        int waiting = curr->start_time - curr->arrival_time;
        int response = waiting;

        total_turnaround += turnaround;
        total_waiting += waiting;
        total_response += response;

        for (int i = 0; i < n; i++) {
            if (&procs[i] == curr) {
                is_completed[i] = 1;
                break;
            }
        }

        completed++;
    }

    printf("\n--- SJF Scheduling ---\n");
    printf("Avg Turnaround Time: %.2f\n", (float)total_turnaround / n);
    printf("Avg Waiting Time   : %.2f\n", (float)total_waiting / n);
    printf("Avg Response Time  : %.2f\n", (float)total_response / n);
    printf("Throughput         : %.2f\n", (float)n / time);
    print_time_chart();
}
