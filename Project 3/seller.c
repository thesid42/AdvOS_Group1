#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "globals.h"

extern void print_seating_chart();
extern int assign_seat_H(char *cust_name);
extern int assign_seat_M(char *cust_name);
extern int assign_seat_L(char *cust_name);
extern Customer* dequeue_customer(Customer **head);

// Get random service time for seller type
int random_service_time(int seller_type) {
    if (seller_type == H_SELLER) return 1 + rand() % 2; // 1 or 2
    if (seller_type == M_SELLER) return 2 + rand() % 3; // 2,3,4
    return 4 + rand() % 4; // 4,5,6,7
}

void *sell(void *arg) {
    Seller *seller = (Seller *)arg;
    int seller_type = seller->type;
    int idx = seller->index;
    int curr_time = 0;
    Customer *curr = seller->queue_head;
    while (curr) {
        // Wait for customer to arrive
        if (curr->arrival_time > curr_time) {
            sleep(curr->arrival_time - curr_time);
            curr_time = curr->arrival_time;
        }
        // Lock seat map
        pthread_mutex_lock(&seat_mutex);
        if (sold_out) {
            pthread_mutex_unlock(&seat_mutex);
            printf("%02d:%02d %s leaves, sold out\n", curr_time/60, curr_time%60, curr->name);
            seller->customers_turned_away++;
            curr = curr->next;
            continue;
        }
        int seat = -1;
        if (seller_type == H_SELLER) seat = assign_seat_H(curr->name);
        else if (seller_type == M_SELLER) seat = assign_seat_M(curr->name);
        else seat = assign_seat_L(curr->name);
        if (seat == -1) {
            sold_out = 1;
            pthread_mutex_unlock(&seat_mutex);
            printf("%02d:%02d %s leaves, sold out\n", curr_time/60, curr_time%60, curr->name);
            seller->customers_turned_away++;
            curr = curr->next;
            continue;
        }
        pthread_mutex_unlock(&seat_mutex);
        // Serve customer
        curr->service_start = curr_time;
        printf("%02d:%02d %s assigned seat %d\n", curr_time/60, curr_time%60, curr->name, seat+1);
        int service_time = random_service_time(seller_type);
        sleep(service_time);
        curr_time += service_time;
        curr->finish_time = curr_time;
        printf("%02d:%02d %s completed purchase\n", curr_time/60, curr_time%60, curr->name);
        seller->customers_served++;
        seller->total_response_time += (curr->service_start - curr->arrival_time);
        seller->total_turnaround_time += (curr->finish_time - curr->arrival_time);
        print_seating_chart();
        curr = curr->next;
    }
    return NULL;
}