#include <stdio.h>
#include "process.h"

void run_fcfs(Process *procs, int n);
void run_sjf(Process *procs, int n);
void run_srt(Process *procs, int n);
void run_rr(Process *procs, int n);


int main() {
    Process procs[MAX_PROCESSES];
    int n;

    // Run FCFS
    generate_processes(procs, &n, 0);
    run_fcfs(procs, n);

    // Run SJF
    generate_processes(procs, &n, 1);
    run_sjf(procs, n);

    // Run SRT
    generate_processes(procs, &n, 2);
    run_srt(procs, n);
    
    // Run RR
    generate_processes(procs, &n, 3);
    run_rr(procs, n);

    return 0;
}
