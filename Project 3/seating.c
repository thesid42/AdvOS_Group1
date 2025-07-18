#include <stdio.h>
#include <string.h>
#include "globals.h"

// H seller: assign from front (row 0) to back (row 9), left to right
int assign_seat_H(char *cust_name) {
    for (int r = 0; r < NUM_ROWS; ++r) {
        for (int c = 0; c < NUM_COLS; ++c) {
            if (seat_map[r][c][0] == 0) {
                strncpy(seat_map[r][c], cust_name, MAX_NAME_LEN);
                return r * NUM_COLS + c;
            }
        }
    }
    return -1;
}

// L seller: assign from back (row 9) to front (row 0), left to right
int assign_seat_L(char *cust_name) {
    for (int r = NUM_ROWS-1; r >= 0; --r) {
        for (int c = 0; c < NUM_COLS; ++c) {
            if (seat_map[r][c][0] == 0) {
                strncpy(seat_map[r][c], cust_name, MAX_NAME_LEN);
                return r * NUM_COLS + c;
            }
        }
    }
    return -1;
}

// M seller: assign in special order: 4,5,3,6,2,7,1,8,0,9 (row indices)
static const int m_order[NUM_ROWS] = {4,5,3,6,2,7,1,8,0,9};
int assign_seat_M(char *cust_name) {
    for (int i = 0; i < NUM_ROWS; ++i) {
        int r = m_order[i];
        for (int c = 0; c < NUM_COLS; ++c) {
            if (seat_map[r][c][0] == 0) {
                strncpy(seat_map[r][c], cust_name, MAX_NAME_LEN);
                return r * NUM_COLS + c;
            }
        }
    }
    return -1;
}

// Print seating chart
void print_seating_chart() {
    printf("\nSeating Chart:\n");
    for (int r = 0; r < NUM_ROWS; ++r) {
        for (int c = 0; c < NUM_COLS; ++c) {
            if (seat_map[r][c][0])
                printf("%s ", seat_map[r][c]);
            else
                printf("---- ");
        }
        printf("\n");
    }
    printf("\n");
}