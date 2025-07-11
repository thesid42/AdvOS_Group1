#include <stdio.h>
#include "process.h"

void run_fcfs(Process *procs, int n) {
    int time = 0;
    int total_turnaround = 0, total_waiting = 0, total_response = 0;

    printf("\n--- FCFS Scheduling ---\n");

    reset_time_chart();  

    for (int i = 0; i < n; i++) {
        if (time < procs[i].arrival_time)
            time = procs[i].arrival_time;

        procs[i].start_time = time;
        procs[i].finish_time = time + procs[i].burst_time;

        
        for (int q = 0; q < procs[i].burst_time; q++) {
            add_to_time_chart(procs[i].name[0]);  
            time++;
        }

        int turnaround = procs[i].finish_time - procs[i].arrival_time;
        int waiting = procs[i].start_time - procs[i].arrival_time;
        int response = waiting;

        total_turnaround += turnaround;
        total_waiting += waiting;
        total_response += response;
    }

    printf("Avg Turnaround Time: %.2f\n", (float)total_turnaround / n);
    printf("Avg Waiting Time   : %.2f\n", (float)total_waiting / n);
    printf("Avg Response Time  : %.2f\n", (float)total_response / n);
    printf("Throughput         : %.2f\n", (float)n / time);

    print_time_chart(); 
}
