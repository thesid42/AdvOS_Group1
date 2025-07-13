#include <stdio.h>
#include "process.h"

AlgorithmStats run_srt(Process *procs, int n, int run_num) {
    int time = 0, completed = 0;
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    int completed_count = 0;
    int first_response[MAX_PROCESSES];
    
    // Initialize first response times
    for (int i = 0; i < n; i++) {
        first_response[i] = -1;
    }

    printf("--- SRT (Shortest Remaining Time First) Scheduling ---\n");
    reset_time_chart();

    while (completed < n && time <= 99) {
        int shortest = -1;
        
        // Find process with shortest remaining time among arrived processes
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time <= time && procs[i].remaining_time > 0) {
                if (shortest == -1 || procs[i].remaining_time < procs[shortest].remaining_time) {
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            // CPU is idle
            add_to_time_chart('.');
            time++;
            continue;
        }

        // Record first response time
        if (!procs[shortest].started) {
            procs[shortest].start_time = time;
            procs[shortest].started = 1;
            first_response[shortest] = time - procs[shortest].arrival_time;
        }

        // Execute for one quantum
        procs[shortest].remaining_time--;
        add_to_time_chart(procs[shortest].name[0]);
        time++;

        // Check if process completed
        if (procs[shortest].remaining_time == 0) {
            procs[shortest].finish_time = time;

            int turnaround = procs[shortest].finish_time - procs[shortest].arrival_time;
            int waiting = turnaround - procs[shortest].burst_time;
            int response = first_response[shortest];

            total_turnaround += turnaround;
            total_waiting += waiting;
            total_response += response;
            completed_count++;
            completed++;
        }
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
