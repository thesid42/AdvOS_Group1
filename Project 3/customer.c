#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

// Insert customer into queue sorted by arrival time
void enqueue_customer(Customer **head, Customer *new_cust) {
    if (!*head || new_cust->arrival_time < (*head)->arrival_time) {
        new_cust->next = *head;
        *head = new_cust;
        return;
    }
    Customer *curr = *head;
    while (curr->next && curr->next->arrival_time <= new_cust->arrival_time) {
        curr = curr->next;
    }
    new_cust->next = curr->next;
    curr->next = new_cust;
}

// Dequeue customer from head
Customer* dequeue_customer(Customer **head) {
    if (!*head) return NULL;
    Customer *c = *head;
    *head = (*head)->next;
    c->next = NULL;
    return c;
}

// Generate random arrival time [0,59]
int random_arrival() {
    return rand() % 60;
}

// Generate customer name (e.g., H001, M101, L201)
void make_customer_name(char *buf, int seller_type, int seller_idx, int cust_num) {
    if (seller_type == H_SELLER) {
        sprintf(buf, "H%03d", cust_num+1);
    } else if (seller_type == M_SELLER) {
        sprintf(buf, "M%d%02d", seller_idx, cust_num+1);
    } else {
        sprintf(buf, "L%d%02d", seller_idx-4+1, cust_num+1);
    }
}

// Create and enqueue N customers for a seller
void create_customers_for_seller(Seller *seller, int seller_type, int seller_idx, int N) {
    for (int i = 0; i < N; ++i) {
        Customer *c = malloc(sizeof(Customer));
        c->id = i;
        c->arrival_time = random_arrival();
        c->service_start = -1;
        c->finish_time = -1;
        make_customer_name(c->name, seller_type, seller_idx, i);
        c->next = NULL;
        enqueue_customer(&seller->queue_head, c);
    }
    seller->total_customers = N;
}