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
		// check if it is time to serve the next customer and that the queue is not empty
		if((clock_time >= q.top().arrival_time) && (!this->isEmpty())) {
			// get the customer from the queue
			Customer c = this->q.top();
			// pop the customer from the queue
			this->q.pop();
			// get the random service time required for the customer
			int serve_time = get_service_time();
			
			// get mutexes to enter critical region
			pthread_mutex_lock(&mutex_sell);
			pthread_mutex_lock(&print_lock);
			pthread_mutex_lock(&tickets_available_mutex);
			// check if there are any available tickets left
			if(0 < tickets_available) {
				print_purchase((clock_time + serve_time), &c, this->seller_type.c_str());
				// print the seating chart
				print_seats(concert_seats);
				// decrement the number of remaining tickets
				tickets_available--;
			}
			else {
				print_soldout(clock_time, &c);
			}
			// release mutexes
			pthread_mutex_unlock(&tickets_available_mutex);
			pthread_mutex_unlock(&print_lock);
			pthread_mutex_unlock(&mutex_sell);
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
void Seller::set_next_free_seat() {
	// check if this is a H ticket seller
	if('H' == this->seller_type[0]) {
		// lock the H row
		pthread_mutex_lock(&rowH_mutex);
		// lock the H seat
		pthread_mutex_lock(&seatH_mutex);
		// increment the seat
		seatH++;
		// check if the seat and row need to be adjusted
		if(9 < seatH) {
			rowH++;
			seatH = 0;
		}
		// unlock the H row
		pthread_mutex_unlock(&rowH_mutex);
		// unlock the H seat
		pthread_mutex_unlock(&seatH_mutex);
		// return from the function
		return;
	}
	// check if this is a M ticket seller
	if('M' == this->seller_type[0]) {
	/*	
  int rowM;
  int seatM;
 pthread_mutex_t rowM_mutex;
 pthread_mutex_t seatM_mutex;
		*/
		return;
	}
	// check if this is a L ticket seller
	if('L' == this->seller_type[0]) {
		// lock the H row
		pthread_mutex_lock(&rowL_mutex);
		// lock the H seat
		pthread_mutex_lock(&seatL_mutex);
		// increment the seat
		seatL++;
		// check if the seat and row need to be adjusted
		if(9 < seatL) {
			rowL--;
			seatL = 0;
		}
		// unlock the H row
		pthread_mutex_unlock(&rowL_mutex);
		// unlock the H seat
		pthread_mutex_unlock(&seatL_mutex);
		// return from the function
		return;
	}
	
	
}
