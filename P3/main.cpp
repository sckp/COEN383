#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// for sleeping
#include <unistd.h>

// thread library
#include <pthread.h>
// cpp queue container
#include <queue>
// for the concert seats will be a string vector
#include <string>

// additional files
#include "customer.h"
#include "helper.h"
#include "seller.h"

// condition for threads to begin
pthread_cond_t cond_go = PTHREAD_COND_INITIALIZER;

// mutex for waiting on condition
pthread_mutex_t mutex_condition = PTHREAD_MUTEX_INITIALIZER;

// mutex for threads editing seat chart
pthread_mutex_t mutex_sell = PTHREAD_MUTEX_INITIALIZER;

// for debugging within the thread, a mutex is needed
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

volatile int clock_time;
int max_time = 60;


void wakeup_all_seller_threads() {
	pthread_mutex_lock(&mutex_condition);
	pthread_cond_broadcast(&cond_go);
	pthread_mutex_unlock(&mutex_condition);
}


int main(int argc, char* argv[]) {
	// initalize the seed for random arrival time
	int seed = time(NULL);
	srand(seed);
	// initialize the clock time to 0
	clock_time = 0;

	// set a default value for the number of customers per queue
	int customers_per_queue = 10;

	// check if the user entered a desired number for customers per queue
	if(2 > argc) {
		printf("No size entered for customers per queue.\nUsing default of 10 customers per queue.\n");
	}
	else {
		// check if the supplied argument is a number
		if(0 < atoi(argv[1])) {
			customers_per_queue =  atoi(argv[1]);
			printf("Each queue will have %i customers.\n", customers_per_queue);
		}
		else {
			printf("Input is not valid.\nUsing default of 10 customers per queue.\n");
		}
	}

	// create the 2d array to represent the 10x10 seating arrangement
	std::string concert_seats[10][10];
	// initialize the seating chart
	initialize_concert(concert_seats);

	// create seller instance array
	Seller* sellers[10];
	// create thread array to store seller threads
	pthread_t tids[10];


	// create sellers and their queues
	sellers[0] = new Seller(concert_seats, "H0", customers_per_queue);
	tids[0] = sellers[0]->getThread();

	for(int i = 1; i < 4; i++) {
		sellers[i] = new Seller(concert_seats, "M" + std::to_string(i), customers_per_queue);
		tids[i] = sellers[i]->getThread();
	}

	for(int i = 4; i < 10; i++) {
		sellers[i] = new Seller(concert_seats, "L" + std::to_string(i - 3), customers_per_queue);
		tids[i] = sellers[i]->getThread();
	}

	// sleep 1 second to make sure all threads have been created and are waiting
	sleep(1);
	wakeup_all_seller_threads();

	// do work on thre threads while incrementing time
	// sleep is here for now as I am unsure how to wait for all of the threads to be waiting on the conditional
	while(clock_time < max_time) {
		sleep(1);
		clock_time++;
	}

	// wait for all seller threads to exit
	for(int i = 0; i < 10; i++) {
		pthread_join(tids[i], NULL);
	}

	// free all of the sellers
	for(int i=0; i<10; i++) {
		free(sellers[i]);
	}

	exit(0);
}
