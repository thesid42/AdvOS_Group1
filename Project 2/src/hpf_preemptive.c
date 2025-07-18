#include <stdio.h>
#include <stdlib.h>
#include "process.h"

#define MAX_QUEUE_SIZE 1000

AlgorithmStats run_hpf_preemptive(Process *procs, int n, int run_num) {
    int time = 0;
    int completed = 0;
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    int completed_count = 0;
    int next_arrival_index = 0;
    
    // Priority queues (1 = highest priority, 4 = lowest priority)
    int priority_queues[5][MAX_QUEUE_SIZE]; // Index 0 unused, 1-4 for priorities
    int front[5] = {0}, rear[5] = {0};
    
    // Priority queue statistics
    int priority_completed[5] = {0};
    int priority_turnaround[5] = {0};
    int priority_waiting[5] = {0};
    int priority_response[5] = {0};
    
    printf("--- HPF Preemptive Scheduling ---\n");
    reset_time_chart();
    
    // Sort processes by arrival time
    qsort(procs, n, sizeof(Process), cmp_arrival);
    
    // Initialize processes
    for (int i = 0; i < n; i++) {
        procs[i].remaining_time = procs[i].burst_time;
        procs[i].started = 0;
        procs[i].start_time = -1;
        procs[i].finish_time = -1;
    }
    
    while (completed < n && time <= 99) {
        // Add newly arrived processes to their priority queues
        while (next_arrival_index < n && procs[next_arrival_index].arrival_time <= time) {
            int priority = procs[next_arrival_index].priority;
            if (rear[priority] < MAX_QUEUE_SIZE) {
                priority_queues[priority][rear[priority]++] = next_arrival_index;
            }
            next_arrival_index++;
        }
        
        // Find the highest priority queue that has processes
        int selected_priority = -1;
        for (int p = 1; p <= 4; p++) {
            if (front[p] < rear[p]) {
                selected_priority = p;
                break;
            }
        }
        
        if (selected_priority == -1) {
            // CPU is idle
            add_to_time_chart('.');
            time++;
            continue;
        }
        
        // Get process from the highest priority queue
        int idx = priority_queues[selected_priority][front[selected_priority]++];
        Process *curr = &procs[idx];
        
        // Record start time for response time calculation
        if (!curr->started) {
            curr->started = 1;
            curr->start_time = time;
        }
        
        // Execute for one time quantum
        curr->remaining_time--;
        add_to_time_chart(curr->name[0]);
        time++;
        
        // Add newly arrived processes to their priority queues
        while (next_arrival_index < n && procs[next_arrival_index].arrival_time <= time) {
            int priority = procs[next_arrival_index].priority;
            if (rear[priority] < MAX_QUEUE_SIZE) {
                priority_queues[priority][rear[priority]++] = next_arrival_index;
            }
            next_arrival_index++;
        }
        
        // Check if process completed
        if (curr->remaining_time == 0) {
            curr->finish_time = time;
            completed++;
            completed_count++;
            
            // Calculate statistics
            int turnaround = curr->finish_time - curr->arrival_time;
            int waiting = turnaround - curr->burst_time;
            int response = curr->start_time - curr->arrival_time;
            
            total_turnaround += turnaround;
            total_waiting += waiting;
            total_response += response;
            
            // Update priority queue statistics
            int priority = curr->priority;
            priority_completed[priority]++;
            priority_turnaround[priority] += turnaround;
            priority_waiting[priority] += waiting;
            priority_response[priority] += response;
        } else {
            // Re-enqueue in the same priority queue (RR within priority level)
            if (rear[selected_priority] < MAX_QUEUE_SIZE) {
                priority_queues[selected_priority][rear[selected_priority]++] = idx;
            }
        }
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
