#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "globals.h"

// Prototypes from other modules
void create_customers_for_seller(Seller *seller, int seller_type, int seller_idx, int N);
void *sell(void *arg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N_customers_per_seller>\n", argv[0]);
        return 1;
    }
    N_customers = atoi(argv[1]);
    if (N_customers < 1 || N_customers > MAX_CUSTOMERS) {
        printf("N must be between 1 and %d\n", MAX_CUSTOMERS);
        return 1;
    }
    srand(time(NULL));
    // Initialize sellers and customer queues
    sellers[0].type = H_SELLER;
    sellers[0].index = 0;
    sellers[0].queue_head = NULL;
    sellers[0].customers_served = 0;
    sellers[0].customers_turned_away = 0;
    sellers[0].total_response_time = 0;
    sellers[0].total_turnaround_time = 0;
    create_customers_for_seller(&sellers[0], H_SELLER, 0, N_customers);
    for (int i = 1; i < 4; ++i) {
        sellers[i].type = M_SELLER;
        sellers[i].index = i;
        sellers[i].queue_head = NULL;
        sellers[i].customers_served = 0;
        sellers[i].customers_turned_away = 0;
        sellers[i].total_response_time = 0;
        sellers[i].total_turnaround_time = 0;
        create_customers_for_seller(&sellers[i], M_SELLER, i, N_customers);
    }
    for (int i = 4; i < 10; ++i) {
        sellers[i].type = L_SELLER;
        sellers[i].index = i;
        sellers[i].queue_head = NULL;
        sellers[i].customers_served = 0;
        sellers[i].customers_turned_away = 0;
        sellers[i].total_response_time = 0;
        sellers[i].total_turnaround_time = 0;
        create_customers_for_seller(&sellers[i], L_SELLER, i, N_customers);
    }
    // Create seller threads
    for (int i = 0; i < NUM_SELLERS; ++i) {
        pthread_create(&sellers[i].thread, NULL, sell, &sellers[i]);
    }
    // Wait for all threads to finish
    for (int i = 0; i < NUM_SELLERS; ++i) {
        pthread_join(sellers[i].thread, NULL);
    }
    // Print statistics
    int h_served=0, m_served=0, l_served=0, h_away=0, m_away=0, l_away=0;
    int h_resp=0, m_resp=0, l_resp=0, h_turn=0, m_turn=0, l_turn=0;
    for (int i = 0; i < NUM_SELLERS; ++i) {
        if (sellers[i].type == H_SELLER) {
            h_served += sellers[i].customers_served;
            h_away += sellers[i].customers_turned_away;
            h_resp += sellers[i].total_response_time;
            h_turn += sellers[i].total_turnaround_time;
        } else if (sellers[i].type == M_SELLER) {
            m_served += sellers[i].customers_served;
            m_away += sellers[i].customers_turned_away;
            m_resp += sellers[i].total_response_time;
            m_turn += sellers[i].total_turnaround_time;
        } else {
            l_served += sellers[i].customers_served;
            l_away += sellers[i].customers_turned_away;
            l_resp += sellers[i].total_response_time;
            l_turn += sellers[i].total_turnaround_time;
        }
    }
    printf("\nSummary:\n");
    printf("H: %d served, %d turned away\n", h_served, h_away);
    printf("M: %d served, %d turned away\n", m_served, m_away);
    printf("L: %d served, %d turned away\n", l_served, l_away);
    if (h_served) printf("H avg response: %.2f, avg turnaround: %.2f\n", (float)h_resp/h_served, (float)h_turn/h_served);
    if (m_served) printf("M avg response: %.2f, avg turnaround: %.2f\n", (float)m_resp/m_served, (float)m_turn/m_served);
    if (l_served) printf("L avg response: %.2f, avg turnaround: %.2f\n", (float)l_resp/l_served, (float)l_turn/l_served);
    return 0;
}