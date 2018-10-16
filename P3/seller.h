#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include <iostream>
#include <pthread.h>

#include "customer.h"
extern pthread_cond_t cond_go;
extern pthread_mutex_t mutex_condition;
extern pthread_mutex_t mutex_sell;
extern pthread_mutex_t print_lock;
extern volatile int clock_time;
extern int max_time;


class Seller {
	private:
		std::string seller_type;
		std::priority_queue<Customer> q;
		// reference to customer seats
		std::string (*concert_seats)[10];
		// reference to thread
		pthread_t my_thread;

	public:
		// constructor
		Seller(std::string seats[][10], std::string seller_type, int queue_size);
		void setSellerType(std::string seller_type);
		void sell();
		void push_queue(Customer c);
		bool isEmpty();
		pthread_t getThread();
		Customer pop_queue();
		// function to get random service time
		int get_service_time();
		// function to fill the sellers queue
		void fill_queue(int n);
};
