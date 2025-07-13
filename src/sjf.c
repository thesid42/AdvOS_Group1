#include <stdio.h>
#include <stdlib.h>
#include "process.h"

extern int cmp_burst(const void *, const void *);

AlgorithmStats run_sjf(Process *procs, int n, int run_num) {
    int time = 0, completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    int completed_count = 0;
    
    printf("--- SJF Scheduling ---\n");
    reset_time_chart();

    while (completed < n && time <= 99) {
        Process *ready[MAX_PROCESSES];
        int ready_count = 0;

        // Find all processes that have arrived and are not completed
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && procs[i].arrival_time <= time)
                ready[ready_count++] = &procs[i];
        }

        if (ready_count == 0) {
            // CPU is idle, increment time
            add_to_time_chart('.');
            time++;
            continue;
        }

        // Sort ready processes by burst time (SJF)
        qsort(ready, ready_count, sizeof(Process *), cmp_burst);
        Process *curr = ready[0];

        // If this is the first time the process gets CPU, set response time
        if (curr->start_time == -1) {
            curr->start_time = time;
        }

        // Execute the process for its full burst time
        for (int q = 0; q < curr->burst_time; q++) {
            add_to_time_chart(curr->name[0]);
            time++;
        } 
        
        curr->finish_time = time;

        // Calculate statistics
        int turnaround = curr->finish_time - curr->arrival_time;
        int waiting = curr->start_time - curr->arrival_time;
        int response = waiting;  // For non-preemptive, response = waiting

        total_turnaround += turnaround;
        total_waiting += waiting;
        total_response += response;
        completed_count++;

        // Mark process as completed
        for (int i = 0; i < n; i++) {
            if (&procs[i] == curr) {
                is_completed[i] = 1;
                break;
            }
        }

        completed++;
    }

    // Calculate averages
    AlgorithmStats stats;
    if (completed_count > 0) {
        stats.avg_turnaround = (double)total_turnaround / completed_count;
        stats.avg_waiting = (double)total_waiting / completed_count;
        stats.avg_response = (double)total_response / completed_count;
        stats.throughput = (double)completed_count / time;
    } else {
        stats.avg_turnaround = 0;
        stats.avg_waiting = 0;
        stats.avg_response = 0;
        stats.throughput = 0;
    }

    printf("Processes completed: %d/%d\n", completed_count, n);
    printf("Avg Turnaround Time: %.2f\n", stats.avg_turnaround);
    printf("Avg Waiting Time   : %.2f\n", stats.avg_waiting);
    printf("Avg Response Time  : %.2f\n", stats.avg_response);
    printf("Throughput         : %.2f\n", stats.throughput);
    print_time_chart();
    
    return stats;
}
