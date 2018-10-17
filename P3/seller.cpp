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
			
			// find out if any tickets are still available
			int tickets_remain;
			pthread_mutex_lock(&tickets_available_mutex);
			tickets_remain = tickets_available;
			pthread_mutex_unlock(&tickets_available_mutex);
			
			pthread_mutex_lock(&mutex_sell);
			pthread_mutex_lock(&print_lock);
			
			if(0 < tickets_remain) {
				print_purchase((clock_time + serve_time), &c, this->seller_type.c_str());
				// decrement the number of remaining tickets
				pthread_mutex_lock(&tickets_available_mutex);
				tickets_available--;
				pthread_mutex_unlock(&tickets_available_mutex);
			}
			else {
				print_soldout(clock_time, &c);
			}
			
			
			
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
