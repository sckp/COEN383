#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// thread library
#include <pthread.h>
// cpp queue container
#include <queue>

// customer files
#include "customer.h"


pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
	
/*
	// print all of the customers
	for(int i = 0; i < 10; i++) {
		printf("Queue %i's customers:\n", i);
		while(!q[i].empty()) {
			printf("\tCustomer: %i\tArrival Time: %i\n", q[i].top().ID, q[i].top().arrival_time);
			q[i].pop();
		}
	}
*/
	
	int i;
	pthread_t tids[10];
	char seller_type;
	
	// create necessary data structures for the simulator
	// create buyers list for each seller ticket queue based on the 
	// N value within an hour and have them in the seller queue
	
	// create 10 threads representing the 10 sellers
	seller_type = 'H';
	pthread_create(&tids[0], NULL, sell, (void*) &seller_type);
	
	seller_type = 'M';
	for(i = 0; i < 4; i++) {
		pthread_create(&tids[i], NULL, sell, (void*) &seller_type);
	}
	
	seller_type = 'L';
	for(i = 4; i < 10; i++) {
		pthread_create(&tids[i], NULL, sell, (void*) &seller_type);
	}
	
	// wakeup all seller threads
	wakeup_all_seller_threads();
	
	// wait for all seller threads to exit
	for(i = 0; i < 10; i++) {
		pthread_join(tids[i], NULL);
	}
	
	// printout simulation results
	exit(0);
}






