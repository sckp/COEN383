#ifndef CUSTOMER_H
#define CUSTOMER_H

typedef struct Customer {
	int ID;
	int arrival_time;
	int row_number;
	int seat_number;
} Customer;

void set_customer_seat(Customer* c, int r, int s);



#endif
