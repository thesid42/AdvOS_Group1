#include <stdio.h>
#include <stdlib.h>
#include "process.h"

AlgorithmStats run_hpf_non_preemptive(Process *procs, int n, int run_num) {
    int time = 0;
    int completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    int completed_count = 0;
    
    // Priority queue statistics
    int priority_completed[5] = {0}; // Index 0 unused, 1-4 for priorities
    int priority_turnaround[5] = {0};
    int priority_waiting[5] = {0};
    int priority_response[5] = {0};
    
    printf("--- HPF Non-preemptive Scheduling ---\n");
    reset_time_chart();
    
    while (completed < n && time <= 99) {
        int selected = -1;
        int highest_priority = 5; // Lower number = higher priority
        
        // Find the highest priority process that has arrived
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && procs[i].arrival_time <= time) {
                if (procs[i].priority < highest_priority) {
                    highest_priority = procs[i].priority;
                    selected = i;
                } else if (procs[i].priority == highest_priority) {
                    // If same priority, use FCFS (first arrived)
                    if (procs[i].arrival_time < procs[selected].arrival_time) {
                        selected = i;
                    }
                }
            }
        }
        
        if (selected == -1) {
            // CPU is idle
            add_to_time_chart('.');
            time++;
            continue;
        }
        
        Process *curr = &procs[selected];
        
        // Set start time (first time getting CPU)
        if (curr->start_time == -1) {
            curr->start_time = time;
        }
        
        // Execute the process for its full burst time (non-preemptive)
        for (int q = 0; q < curr->burst_time; q++) {
            add_to_time_chart(curr->name[0]);
            time++;
        }
        
        curr->finish_time = time;
        is_completed[selected] = 1;
        completed++;
        completed_count++;
        
        // Calculate statistics
        int turnaround = curr->finish_time - curr->arrival_time;
        int waiting = curr->start_time - curr->arrival_time;
        int response = waiting; // For non-preemptive, response = waiting
        
        total_turnaround += turnaround;
        total_waiting += waiting;
        total_response += response;
        
        // Update priority queue statistics
        int priority = curr->priority;
        priority_completed[priority]++;
        priority_turnaround[priority] += turnaround;
        priority_waiting[priority] += waiting;
        priority_response[priority] += response;
    }
    
    // Calculate overall averages
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
    printf("Overall Statistics:\n");
    printf("Avg Turnaround Time: %.2f\n", stats.avg_turnaround);
    printf("Avg Waiting Time   : %.2f\n", stats.avg_waiting);
    printf("Avg Response Time  : %.2f\n", stats.avg_response);
    printf("Throughput         : %.2f\n", stats.throughput);
    
    // Print priority queue statistics
    printf("\nPer-Priority Queue Statistics:\n");
    for (int p = 1; p <= 4; p++) {
        printf("Priority %d: ", p);
        if (priority_completed[p] > 0) {
            printf("Completed: %d, Avg TAT: %.2f, Avg Wait: %.2f, Avg Response: %.2f, Throughput: %.2f\n",
                   priority_completed[p],
                   (double)priority_turnaround[p] / priority_completed[p],
                   (double)priority_waiting[p] / priority_completed[p],
                   (double)priority_response[p] / priority_completed[p],
                   (double)priority_completed[p] / time);
        } else {
            printf("Completed: 0 (possible starvation)\n");
        }
    }
    
    print_time_chart();
    return stats;
} 
