#include "helper.h"

// function to initialize the concert seats
void initialize_concert(std::string c[10][10]) {
	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			c[i][j] = "-";
		}
	}
}

// function to print the concert seating
void print_seats(std::string c[10][10]) {
	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			printf("%s ", c[i][j].c_str());
		}
		// create a new line
		printf("\n");
	}
}

// function to print a customer purchasing a ticket and leaving
void print_purchase(int clock_time, Customer* c, const char* s) {
	printf("0:%02i Customer %i has purchased a ticket from seller %s and left.\n", clock_time, c->ID, s);
}

// function to print that the concert is soldout and the customer is turned away
void print_soldout(int clock_time, Customer* c) {
	printf("0:%02i Customer %i has been turned away because concert is sold out.\n", clock_time, c->ID);
}
