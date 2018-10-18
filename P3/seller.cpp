#include "seller.h"

// static function to start threading from within the constructor
static void* sell_x(void* object) {
	return ((Seller*) object)->sell();
}

// 3 argument constructor
Seller::Seller(std::string seats[][10], std::string seller_type, int queue_size) {
	concert_seats = seats;
	this->seller_type = seller_type;
	remaining_customer_service_time = 0;
	// call function to populate the customer queue
	fill_queue(queue_size);

	// pass pthread_create "this" to call a member function in sell_x
	pthread_create(&my_thread, NULL, sell_x, (void*) this);
}


void* Seller::sell() {
	pthread_mutex_lock(&mutex_condition);
	pthread_cond_wait(&cond_go, &mutex_condition);
	pthread_mutex_unlock(&mutex_condition);
	while(clock_time < max_time) {
		int ready_for_customer = 0;
		// check if it is time to serve the next customer and that the queue is not empty
		if((clock_time >= q.top().arrival_time) && (!this->isEmpty()) && (clock_time >= ready_for_customer)) {
				
			// get the customer from the queue
			Customer c = this->q.top();
			// pop the customer from the queue
			this->q.pop();
			// get the random service time required for the customer
			int serve_time = get_service_time();
			
			// get mutexes to enter critical region
			pthread_mutex_lock(&print_lock);
			pthread_mutex_lock(&mutex_sell);
			pthread_mutex_lock(&seating_index_mutex);
			pthread_mutex_lock(&tickets_available_mutex);
			// check if there are any available tickets left
			if(0 < tickets_available) {
				// customer is being serviced so set next available time
				ready_for_customer = (clock_time + serve_time);
				// decrement the number of remaining tickets
				tickets_available--;
				// variables to hold the row and seat indices
				int avail_row;
				int avail_seat;
				// get the next available seat and make sure it isn't already taken
				do {
					// get the available row
					avail_row = get_row();
					// get the available seat
					avail_seat = get_seat();
					// set the next available seat if there are still tickets available
					set_next_free_seat();
					// put in an exit condition for the threads to prevent infinite loop looking for seat
					if(('H' == this->seller_type[0]) && (9 < rowH)) {
						break;
					}
					else if(('M' == this->seller_type[0]) && (0 > rowM)) {
						break;
					}
					else if(('L' == this->seller_type[0]) && (0 > rowL)) {
						break;
					}
					
					
				} while ("-" != concert_seats[avail_row][avail_seat]);
				
				// print the purchase
				print_purchase((clock_time + serve_time), &c, this->seller_type.c_str());
				// place the customer in the seat
				concert_seats[avail_row][avail_seat] = (seller_type + "0" + std::to_string(c.ID));
				// print the seating chart
				print_seats(concert_seats);
				// create a space for the output
				printf("\n");
				// increment the number of customers that got seats
				if('H' == this->seller_type[0]) {
					seated_customers_H++;
				}
				else if('M' == this->seller_type[0]) {
					seated_customers_M++;
				}
				else if('L' == this->seller_type[0]) {
					seated_customers_L++;
				}
			}
			else {
				print_soldout(clock_time, &c, this->seller_type.c_str());
				// increment the number of customers turned away
				turned_away_customers++;
			}
			// release mutexes
			pthread_mutex_unlock(&tickets_available_mutex);
			pthread_mutex_unlock(&seating_index_mutex);
			pthread_mutex_unlock(&mutex_sell);
			pthread_mutex_unlock(&print_lock);
		}
	}
	return NULL;
}

// checks if the seller queue is empty
bool Seller::isEmpty() {
	return q.empty();
}

// sets the seller type
void Seller::setSellerType(std::string seller_type) {
	this->seller_type = seller_type;
}

// add a customer to the seller's queue
void Seller::push_queue(Customer c) {
	q.push(c);
}

// remove a customer from the sellers queue
void Seller::pop_queue() {
	q.pop();
}

// function to return the thread
pthread_t Seller::getThread() {
  return my_thread;
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

// function to populate the sellers queue
void Seller::fill_queue(int n) {
	for(int i = 0; i < n; i++) {
		Customer c;
		generate_customer(&c, i);
		q.push(c);
	}
}

// function that sets the next free seat
// requires getting the lock prior to its call
void Seller::set_next_free_seat() {
	// check if this is a H ticket seller
	if('H' == this->seller_type[0]) {
		// increment the seat
		seatH++;
		// check if the seat and row need to be adjusted
		if(9 < seatH) {
			seatH = 0;
			rowH++;
		}
	}
	// check if this is a M ticket seller
	else if('M' == this->seller_type[0]) {
		// increment the seat
		seatM++;
		// check if the seat and row need to be adjusted
		if(9 < seatM) {
			seatM = 0;
			// determine which row to begin assigning next
			if(5 == rowM) {
				rowM = 6;
			}
			else if(6 == rowM) {
				rowM = 4;
			}
			else if(4 == rowM) {
				rowM = 7;
			}
			else if(7 == rowM) {
				rowM = 3;
			}
			else if(3 == rowM) {
				rowM = 8;
			}
			else if(8 == rowM) {
				rowM = 2;
			}
			else if(2 == rowM) {
				rowM = 9;
			}
			else if(9 == rowM) {
				rowM = 1;
			}
			else if(1 == rowM) {
				rowM = 0;
			}
			else if(0 == rowM) {
				rowM == -1;
			}
		}
	}
	// check if this is a L ticket seller
	else if('L' == this->seller_type[0]) {
		// increment the seat
		seatL++;
		// check if the seat and row need to be adjusted
		if(9 < seatL) {
			seatL = 0;
			rowL--;
		}
	}
}

// get the current row for seating
// requires getting the lock prior to its call
int Seller::get_row() {
	int r;
	// look at which row index to get
	if('H' == this->seller_type[0]) {
		r = rowH;
	}
	else if('M' == this->seller_type[0]) {
		r = rowM;
	}
	else if('L' == this->seller_type[0]) {
		r = rowL;
	}
	// return the row index
	return r;
}

// get the current seat to be used
// requires getting the lock prior to its call
int Seller::get_seat() {
	int s;
	// look at which seat index to get
	if('H' == this->seller_type[0]) {
		s = seatH;
	}
	else if('M' == this->seller_type[0]) {
		s = seatM;
	}
	else if('L' == this->seller_type[0]) {
		s = seatL;
	}
	// return the seat index
	return s;
}


