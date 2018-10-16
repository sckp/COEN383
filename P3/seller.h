#ifndef SELLER_H
#define SELLER_H

#include <string>
#include <queue>
#include <time.h>
#include "customer.h"

class Seller {
	private:
		std::string seller_type;
		std::priority_queue<Customer> customer_queue;
	
	public:
		// default constructor
		Seller();
		// 1 argument constructor
		Seller(std::string s);
		// 2 argument constructor
		Seller(std::string s, int n);
		// setter for seller type
		void set_seller_type(std::string s);
		// function to fill the sellers queue
		void fill_queue(int n);
		// function to initialize seller without using the constructor
		void initialize_seller(std::string s, int n);
		// function to get random service time
		int get_service_time();
		// function for selling tickets
		void* sell();
		// function to print seller queue
		void print_seller_queue();
};


#endif
