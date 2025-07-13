#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"

AlgorithmStats run_fcfs(Process *procs, int n, int run_num);
AlgorithmStats run_sjf(Process *procs, int n, int run_num);
AlgorithmStats run_srt(Process *procs, int n, int run_num);
AlgorithmStats run_rr(Process *procs, int n, int run_num);

int main() {
    Process procs[MAX_PROCESSES];
    int n;
    const int NUM_RUNS = 5;
    AlgorithmStats fcfs_stats[NUM_RUNS], sjf_stats[NUM_RUNS];
    AlgorithmStats srt_stats[NUM_RUNS], rr_stats[NUM_RUNS];
    
    printf("Process Scheduling Algorithms Simulation\n");
    printf("========================================\n\n");
    
    // Run FCFS 5 times
    printf("Running FCFS Algorithm...\n");
    for (int i = 0; i < NUM_RUNS; i++) {
        printf("\n--- FCFS Run %d ---\n", i + 1);
        generate_processes(procs, &n, i);
        print_processes(procs, n);
        fcfs_stats[i] = run_fcfs(procs, n, i + 1);
    }
    print_algorithm_summary("FCFS", fcfs_stats, NUM_RUNS);
    
    // Run SJF 5 times
    printf("\nRunning SJF Algorithm...\n");
    for (int i = 0; i < NUM_RUNS; i++) {
        printf("\n--- SJF Run %d ---\n", i + 1);
        generate_processes(procs, &n, i + NUM_RUNS);
        print_processes(procs, n);
        sjf_stats[i] = run_sjf(procs, n, i + 1);
    }
    print_algorithm_summary("SJF", sjf_stats, NUM_RUNS);
    
    // Run SRT 5 times
    printf("\nRunning SRT Algorithm...\n");
    for (int i = 0; i < NUM_RUNS; i++) {
        printf("\n--- SRT Run %d ---\n", i + 1);
        generate_processes(procs, &n, i + 2 * NUM_RUNS);
        print_processes(procs, n);
        srt_stats[i] = run_srt(procs, n, i + 1);
    }
    print_algorithm_summary("SRT", srt_stats, NUM_RUNS);
    
    // Run RR 5 times
    printf("\nRunning RR Algorithm...\n");
    for (int i = 0; i < NUM_RUNS; i++) {
        printf("\n--- RR Run %d ---\n", i + 1);
        generate_processes(procs, &n, i + 3 * NUM_RUNS);
        print_processes(procs, n);
        rr_stats[i] = run_rr(procs, n, i + 1);
    }
    print_algorithm_summary("RR", rr_stats, NUM_RUNS);

    return 0;
}
