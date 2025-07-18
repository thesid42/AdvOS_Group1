#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

#define TIME_QUANTUM 1
#define MAX_QUEUE_SIZE 1000

extern int cmp_arrival(const void *, const void *);

AlgorithmStats run_rr(Process *procs, int n, int run_num) {
    int time = 0;
    int completed = 0;
    int queue[MAX_QUEUE_SIZE];
    int front = 0, rear = 0;
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    int completed_count = 0;
    int next_arrival_index = 0;

    printf("--- RR (Round Robin) Scheduling ---\n");
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

    // Main RR scheduling loop
    while (completed < n && time <= 99) {
        // Add newly arrived processes to queue
        while (next_arrival_index < n && procs[next_arrival_index].arrival_time <= time) {
            if (rear < MAX_QUEUE_SIZE) {
                queue[rear++] = next_arrival_index;
            }
            next_arrival_index++;
        }

        // If no processes in queue, CPU is idle
        if (front == rear) {
            add_to_time_chart('.');
            time++;
            continue;
        }

        // Get next process from queue
        int idx = queue[front++];
        Process *p = &procs[idx];

        // Record start time for response time calculation
        if (!p->started) {
            p->started = 1;
            p->start_time = time;
        }

        // Execute for one time quantum
        p->remaining_time--;
        add_to_time_chart(p->name[0]);
        time++;

        // Add newly arrived processes to queue (after current process executed)
        while (next_arrival_index < n && procs[next_arrival_index].arrival_time <= time) {
            if (rear < MAX_QUEUE_SIZE) {
                queue[rear++] = next_arrival_index;
            }
            next_arrival_index++;
        }

        // Check if process completed
        if (p->remaining_time == 0) {
            p->finish_time = time;
            completed++;
            completed_count++;
        } else {
            // Re-enqueue if not finished
            if (rear < MAX_QUEUE_SIZE) {
                queue[rear++] = idx;
            }
        }
    }

    // Calculate statistics for completed processes
    for (int i = 0; i < n; i++) {
        if (procs[i].finish_time != -1) {
            int turnaround = procs[i].finish_time - procs[i].arrival_time;
            int waiting = turnaround - procs[i].burst_time;
            int response = procs[i].start_time - procs[i].arrival_time;
            
            total_turnaround += turnaround;
            total_waiting += waiting;
            total_response += response;
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
