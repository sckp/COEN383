#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include <iostream>
#include <pthread.h>

#include "customer.h"
extern pthread_cond_t cond_go;
extern pthread_mutex_t mutex_condition;
extern pthread_mutex_t mutex_sell;
extern pthread_mutex_t print_lock;
extern volatile int clock_time;
extern int max_time;


class Seller {
  char seller_type;
  std::priority_queue<Customer> q;
  // reference to customer seats
  std::string (*concert_seats)[10];
  // reference to thread
  pthread_t my_thread;

public:
  Seller(std::string seats[][10], char seller_type);
  void setSellerType(char seller_type);
  void sell();
  void push_queue(Customer c);
  bool isEmpty();
  pthread_t getThread();
  Customer pop_queue();
};
