#include "globals.h"

char seat_map[NUM_ROWS][NUM_COLS][MAX_NAME_LEN] = {{{0}}};
pthread_mutex_t seat_mutex = PTHREAD_MUTEX_INITIALIZER;

Seller sellers[NUM_SELLERS];

int sim_clock = 0;
pthread_mutex_t clock_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int N_customers = 0;
int sold_out = 0;