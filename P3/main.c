#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
	 

int main() {
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






