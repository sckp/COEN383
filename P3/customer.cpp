#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "customer.h"



bool operator<(const Customer& c, const Customer& d) {
	if(c.arrival_time > d.arrival_time) {
		return true;
	}
	return false;
}

bool operator<=(const Customer& c, const Customer& d) {
		if(c.arrival_time >= d.arrival_time) {
		return true;
	}
	return false;
}



// initialize the customer data
void generate_customer(Customer *c, int id) {
	c->ID = id;
	c->arrival_time = rand() % 60;
	// use -1 to represent the customer doesn't have a seat
	c->row_number = -1;
	c->seat_number = -1;
}




// set the customer's seat assignment
void set_customer_seat(Customer* c, int r, int s) {
	c->row_number = r;
	c->seat_number = s;
}
