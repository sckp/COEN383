#include "seller.h"

// static function to start threading from within the constructor
static void* sell_x(void* object) {
  return ((Seller*) object)->sell();
}

Seller::Seller(std::string seats[][10], std::string seller_type, int queue_size) {
  concert_seats = seats;
  this->seller_type = seller_type;
  // call function to populate the customer queue
  fill_queue(queue_size);

  // pass pthread_create "this" to call a member function in sell_x
  pthread_create(&my_thread, NULL, sell_x, (void*) this);
}


void* Seller::sell() {
	pthread_mutex_lock(&mutex_condition);
	pthread_cond_wait(&cond_go, &mutex_condition);
	pthread_mutex_unlock(&mutex_condition);
	while(!isEmpty() && clock_time < max_time) {
    while(clock_time < q.top().arrival_time) {

		}
		pthread_mutex_lock(&mutex_sell);
    pthread_mutex_lock(&print_lock);
    std::cout << std::endl;
    std::cout << clock_time << std::endl;
    std::cout << q.top().arrival_time << std::endl;
    std::cout << q.top().ID << std::endl;
    std::cout << seller_type << std::endl;
    std::cout << std::endl;

    /*
      Do logic for dispencing tickets
    int lowSale, highSale;
    **increment current sale time need to find variable**
    if (seller_type == "H") {
    	lowSale = 1;
	highSale = 2;
    } else if (seller_type == "M") {
    	lowSale = 2;
	highSale = 4;
    } else {
    	lowSale = 7;
	highSale = 4;
    }
    if (current sale time == highSale || (current sale time >= lowSale && rand()%2 == 0)) {

    }
    */


    pthread_mutex_unlock(&print_lock);
    q.pop();
		pthread_mutex_unlock(&mutex_sell);
	}
  return NULL;
}

bool Seller::isEmpty() {
  if(this->q.empty() == true) {
    return true;
  } else {
    return false;
  }
}

void Seller::setSellerType(std::string seller_type) {
  this->seller_type = seller_type;
}

void Seller::push_queue(Customer c) {
  this->q.push(c);
}

Customer Seller::pop_queue() {
  this->q.pop();
}

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
