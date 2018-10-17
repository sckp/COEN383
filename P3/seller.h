#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include <iostream>
#include <pthread.h>

#include "customer.h"
#include "helper.h"

extern pthread_cond_t cond_go;
extern pthread_mutex_t mutex_condition;
extern pthread_mutex_t mutex_sell;
extern pthread_mutex_t print_lock;
extern volatile int clock_time;
extern int max_time;

// create a variable to tell when all the tickets have been sold
extern volatile int tickets_available;
extern pthread_mutex_t tickets_available_mutex;
extern volatile int rowH;
extern volatile int seatH;
extern volatile int rowM;
extern volatile int seatM;
extern volatile int rowL;
extern volatile int seatL;
extern pthread_mutex_t seating_index_mutex;

class Seller {
	private:
		// seller type
		std::string seller_type;
		// sellers customer queue
		std::priority_queue<Customer> q;
		// reference to customer seats
		std::string (*concert_seats)[10];
		// reference to thread
		pthread_t my_thread;
		// remaining customer service time
		int remaining_customer_service_time;

	public:
		// constructor
		Seller(std::string seats[][10], std::string seller_type, int queue_size);
		// sets the seller type
		void setSellerType(std::string seller_type);
		void* sell();
		// add a customer to the seller's queue
		void push_queue(Customer c);
		// checks if the seller queue is empty
		bool isEmpty();
		// function to return the thread
		pthread_t getThread();
		// remove a customer from the sellers queue
		void pop_queue();
		// function to get random service time
		int get_service_time();
		// function to fill the sellers queue
		void fill_queue(int n);
		// get the current row for seating
		int get_row();
		// get the current seat to be used
		int get_seat();
		// set the next free seat
		void set_next_free_seat();
};
