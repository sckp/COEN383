#ifndef GLOBAL
#define GLOBAL
#include "pthread.h"
#include <stdbool.h>
#include <sys/time.h>

#define TOTAL_SEATS 100 /// Total seat number
#define MAX_MINUTES 60 /// Total selling
#define NUM_OF_H_SELLERS 1 /// High  sellers
#define NUM_OF_M_SELLERS 3 /// medium  sellers
#define NUM_OF_L_SELLERS 6 /// low  sellers

struct itimerval sellerTimer;

/// Thread paramater
pthread_mutex_t seat_access;
pthread_cond_t cond;
time_t startTime;


int filled_seats; ///Counts how many seats are filled
char* seat_map; /// Seats Map

bool start;
int NUM_OF_CUSTOMERS;

int totalL_sold; /// total tickets L seller sold
int totalM_sold; /// total tickets M seller sold
int totalH_sold; /// total tickets M seller sold


#endif