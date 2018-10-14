#ifndef CUSTOMER_H
#define CUSTOMER_H

// create the customer data type
typedef struct Customer {
	int ID;
	int arrival_time;
	int row_number;
	int seat_number;
	
	
} Customer;

bool operator<(const Customer& c, const Customer& d);
bool operator<=(const Customer& c, const Customer& d);

// initialize the customer data
void generate_customer(Customer *c, int id);

// set the customer's seat assignment
void set_customer_seat(Customer* c, int r, int s);



#endif
