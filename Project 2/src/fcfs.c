#include <stdio.h>
#include "process.h"

AlgorithmStats run_fcfs(Process *procs, int n, int run_num) {
    int time = 0;
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    int completed_count = 0;
    
    printf("--- FCFS Scheduling ---\n");
    reset_time_chart();
    
    int current_process = 0;
    
    while (current_process < n && time <= 99) {
        // Handle idle time until next process arrives
        while (current_process < n && procs[current_process].arrival_time > time) {
            add_to_time_chart('.');  // Idle time
            time++;
        }
        
        // If we've exceeded quantum 99, stop scheduling new processes
        if (time > 99) break;
        
        // Process the current job
        if (current_process < n) {
            Process *curr = &procs[current_process];
            
            // Set start time (first time getting CPU)
            curr->start_time = time;
            
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
        }
        
        current_process++;
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
