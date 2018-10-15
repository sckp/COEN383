#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// thread library
#include <pthread.h>
// cpp queue container
#include <queue>
// for the concert seats will be a string array
#include <string>

// additional files
#include "customer.h"
#include "helper.h"

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int clock_time;

// seller thread to serve one time slice (1 minute)
void* sell(void* s_type) {
	//char* seller_type = (char*) s_type;
	/*
	while(having more work todo) {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		pthread_mutex_unlock(&mutex);
		// serve any buyer available in this seller queue that is ready
		// now to buy ticket till done with all relevant buyers in their queue
		.......
	}
	*/
	return NULL; // thread exits
}

void wakeup_all_seller_threads() {
	pthread_mutex_lock(&mutex);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
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
	// create a priority queue for each seller
	std::priority_queue<Customer> q[10];
	
	// populate each queue
	for(int i = 0; i < 10; i++) {
		// generate each customer for the queue
		for(int j = 0; j < customers_per_queue; j++) {
			Customer c;
			generate_customer(&c, j);
			q[i].push(c);
		}
	}
	
	
	concert_seats[0][4] = '9';
	concert_seats[2][3] = '8';
	concert_seats[5][7] = '7';
	concert_seats[9][9] = '4';
	
	
	
	pthread_t tids[10];
	char seller_type;
	
	// create necessary data structures for the simulator
	// create buyers list for each seller ticket queue based on the 
	// N value within an hour and have them in the seller queue
	
	// create 10 threads representing the 10 sellers
	seller_type = 'H';
	pthread_create(&tids[0], NULL, sell, (void*) &seller_type);
	
	seller_type = 'M';
	for(int i = 0; i < 4; i++) {
		pthread_create(&tids[i], NULL, sell, (void*) &seller_type);
	}
	
	seller_type = 'L';
	for(int i = 4; i < 10; i++) {
		pthread_create(&tids[i], NULL, sell, (void*) &seller_type);
	}
	
	// wakeup all seller threads
	wakeup_all_seller_threads();
	
	// wait for all seller threads to exit
	for(int i = 0; i < 10; i++) {
		pthread_join(tids[i], NULL);
	}
	
	
	
	Customer d;
	generate_customer(&d, 19);
	
	clock_time = 9;
	print_purchase(clock_time, &d);
	
	// print out the concert seating
	print_seats(concert_seats);
	
	exit(0);
}






