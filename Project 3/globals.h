#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>

#define NUM_ROWS 10
#define NUM_COLS 10
#define NUM_SELLERS 10
#define MAX_CUSTOMERS 100
#define MAX_NAME_LEN 8

// Seller types
#define H_SELLER 0
#define M_SELLER 1
#define L_SELLER 2

// Customer struct
typedef struct Customer {
    int id;
    int arrival_time;
    int service_start;
    int finish_time;
    char name[MAX_NAME_LEN];
    struct Customer *next;
} Customer;

// Seller struct
typedef struct Seller {
    int type; // H, M, L
    int index; // 0-9
    Customer *queue_head;
    pthread_t thread;
    int customers_served;
    int customers_turned_away;
    int total_response_time;
    int total_turnaround_time;
    int total_customers;
} Seller;

// Seat map
extern char seat_map[NUM_ROWS][NUM_COLS][MAX_NAME_LEN];
extern pthread_mutex_t seat_mutex;

// Sellers array
extern Seller sellers[NUM_SELLERS];

// Simulation clock
extern int sim_clock;
extern pthread_mutex_t clock_mutex;
extern pthread_cond_t cond;

// Simulation parameters
extern int N_customers;
extern int sold_out;

#endif