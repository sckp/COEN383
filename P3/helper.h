#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <string>
#include "customer.h"

// function to initialize the concert seats
void initialize_concert(std::string c[10][10]);

// function to print the concert seating
void print_seats(std::string c[10][10]);

// function to print a customer purchasing a ticket and leaving
void print_purchase(int clock_time, Customer* c, const char* s);

// function to print that the concert is soldout and the customer is turned away
void print_soldout(int clock_time, Customer* c, const char* s);

#endif
