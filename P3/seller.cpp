#include "seller.h"

// static function to start threading from within the constructor
static void* sell_x(void* object) {
  ((Seller*) object)->sell();
  return NULL;
}

Seller::Seller(std::string seats[][10], char seller_type) {
  concert_seats = seats;
  this->seller_type = seller_type;
  // pass pthread_create "this" to call a member function in sell_x
  pthread_create(&my_thread, NULL, sellx, (void*) this);
}


void Seller::sell() {
	pthread_mutex_lock(&mutex_condition);
	pthread_cond_wait(&cond_go, &mutex_condition);
	pthread_mutex_unlock(&mutex_condition);
	while(!isEmpty() && clock_time < max_time) {
    while(clock_time < q.top().arrival_time) {

		}
		pthread_mutex_lock(&mutex_sell);
    pthread_mutex_lock(&print_lock);
    // std::cout << std::endl;
    // std::cout << clock_time << std::endl;
    // std::cout << q.top().arrival_time << std::endl;
    // std::cout << q.top().ID << std::endl;
    // std::cout << seller_type << std::endl;
    // std::cout << std::endl;

    /*
      Do logic for dispencing tickets
    */


    pthread_mutex_unlock(&print_lock);
    q.pop();
		pthread_mutex_unlock(&mutex_sell);
	}
}

bool Seller::isEmpty() {
  if(this->q.empty() == true) {
    return true;
  } else {
    return false;
  }
}

void Seller::setSellerType(char seller_type) {
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
