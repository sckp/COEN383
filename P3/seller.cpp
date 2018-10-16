#include "seller.h"

// default constructor for seller class
Seller::Seller() {
	seller_type = "";
	
}

// constructor with one argument
Seller::Seller(std::string s) {
	seller_type = s;
}

// 2 argument constructor
Seller::Seller(std::string s, int n) {
	seller_type = s;
	fill_queue(n);
}

// setter function to set the seller type
void Seller::set_seller_type(std::string s) {
	seller_type = s;
}

// function to populate the sellers queue
void Seller::fill_queue(int n) {
	for(int i = 0; i < n; i++) {
		Customer c;
		generate_customer(&c, i);
		customer_queue.push(c);
	}
}

// function to initialize seller without using the constructor
void Seller::initialize_seller(std::string s, int n) {
	seller_type = s;
	fill_queue(n);
}

// function to get random service time
int Seller::get_service_time() {
	// for H: 1-2 minutes
	if('H' == seller_type[0]) {
		return ((rand() % 2) + 1);
	}
	// for M: 2, 3, or 4 minutes
	else if('M' == seller_type[0]) {
		return ((rand() % 3) + 2);
	}
	// for L: 4, 5, 6, or 7 minutes
	else if('L' == seller_type[0]) {
		return ((rand() % 4) + 4);
	}
}

// function for selling tickets
void* Seller::sell() {
	
}

// function to print seller queue
void Seller::print_seller_queue() {
	printf("Seller: %s\n", seller_type.c_str());
	while(!customer_queue.empty()) {
		printf("ID: %i\tArrival Time: %i\n", customer_queue.top().ID, customer_queue.top().arrival_time);
		customer_queue.pop();
	}
	printf("\n");
}




