#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

#define TIME_QUANTUM 1
#define MAX_QUEUE_SIZE 1000
#define MAX_RUNTIME 2000

extern int cmp_arrival(const void *, const void *);

void run_rr(Process *procs, int n) {
    int time = 0;
    int completed = 0;
    int queue[MAX_QUEUE_SIZE]; // Circular queue for ready processes
    int front = 0, rear = 0;
    int i;

    // Sort processes by arrival time
    qsort(procs, n, sizeof(Process), cmp_arrival);

    // Initialize each process
    for (i = 0; i < n; i++) {
        procs[i].remaining_time = procs[i].burst_time;
        procs[i].started = 0;
    }

    // Enqueue processes that arrive at time 0
    for (i = 0; i < n; i++) {
        if (procs[i].arrival_time == 0) {
            queue[rear++] = i;
        }
    }

    // Main RR-scheduling loop
    while (completed < n && time < MAX_RUNTIME) {
        if (front == rear) {
            time++;
            for (i = 0; i < n; i++) {
                if (procs[i].arrival_time == time) {
                    if (rear < MAX_QUEUE_SIZE) {
                        queue[rear++] = i;
                    }
                }
            }
            continue;
        }

        int idx = queue[front++];
        Process *p = &procs[idx];

        // First time scheduled: record start time
        if (p->started == 0) {
            p->started = 1;
            p->start_time = time;
        }

        // Run for one quantum
        p->remaining_time--;
        time++;

        // Enqueue newly arrived processes
        for (i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                if (rear < MAX_QUEUE_SIZE) {
                    queue[rear++] = i;
                }
            }
        }

        // If not finished, re-enqueue the current process
        if (p->remaining_time > 0) {
            if (rear < MAX_QUEUE_SIZE) {
                queue[rear++] = idx;
            }
        } else {
            p->finish_time = time;
            completed++;
        }
    }

    double total_turnaround = 0, total_waiting = 0, total_response = 0;
    for (i = 0; i < n; i++) {
        int turnaround = procs[i].finish_time - procs[i].arrival_time;
        int waiting = turnaround - procs[i].burst_time;
        int response = procs[i].start_time - procs[i].arrival_time;
        total_turnaround += turnaround;
        total_waiting += waiting;
        total_response += response;
    }

    printf("\n--- RR Scheduling ---\n");
    printf("Avg Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Avg Waiting Time   : %.2f\n", total_waiting / n);
    printf("Avg Response Time  : %.2f\n", total_response / n);
    printf("Throughput         : %.2f\n", (double)n / time);
}
