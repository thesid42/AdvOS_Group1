#include <stdio.h>
#include "process.h"

void run_srt(Process *procs, int n) {
    int time = 0, completed = 0;
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    int current = -1;
    int first_response[MAX_PROCESSES] = {0};

    printf("\n--- SRT (Shortest Remaining Time First) Scheduling ---\n");
    reset_time_chart();  // Clear time chart at the beginning

    while (completed < n && time <= 100) {
        int shortest = -1;
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time <= time && procs[i].remaining_time > 0) {
                if (shortest == -1 || procs[i].remaining_time < procs[shortest].remaining_time) {
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        if (!procs[shortest].started) {
            procs[shortest].start_time = time;
            procs[shortest].started = 1;
            first_response[shortest] = time - procs[shortest].arrival_time;
        }

        procs[shortest].remaining_time--;
        add_to_time_chart(procs[shortest].name[0]);  // Track time chart
        time++;

        if (procs[shortest].remaining_time == 0) {
            procs[shortest].finish_time = time;

            int turnaround = procs[shortest].finish_time - procs[shortest].arrival_time;
            int waiting = turnaround - procs[shortest].burst_time;
            int response = first_response[shortest];

            total_turnaround += turnaround;
            total_waiting += waiting;
            total_response += response;

            completed++;
        }
    }

    printf("Avg Turnaround Time: %.2f\n", (float)total_turnaround / n);
    printf("Avg Waiting Time   : %.2f\n", (float)total_waiting / n);
    printf("Avg Response Time  : %.2f\n", (float)total_response / n);
    printf("Throughput         : %.2f\n", (float)n / time);
    print_time_chart();  // Print final time chart
}
