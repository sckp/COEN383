#ifndef SELLER
#define SELLER

#include "customer.h"

struct seller{
    char name[2]; /// Seller's name
    customer* start_queue; /// Customers Queue
};

typedef struct seller seller;

/**
 * Simulate the sealing process
 */
void* sell_seats(void* seat_seller);

/**
 * Print the Seller's name
 */
void print_sellers(seller* list, int length);

/**
 * Prints the seat slots
 */
void print_seats();

/**
 * Print stats when finished
 */
void print_stats();
#endif