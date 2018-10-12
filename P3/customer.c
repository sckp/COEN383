#include "customer.h"


void set_customer_seat(Customer* c, int r, int s) {
	c->row_number = r;
	c->seat_number = s;
}
