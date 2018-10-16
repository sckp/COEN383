#ifndef CUSTOMER
#define CUSTOMER


struct customer{
    char customer_id[2]; // Customer ID
    int arrival_time; // Arrival time
};

typedef struct customer customer;

int NUM_OF_CUSTOMERS; // Total number of customers

/**
 * Initialize the customers queue
 */
void make_customers(customer** queue, int length);

/**
 * Print the customer queue
 */
void print_customers(char* seller_name, customer* list, int length);

/**
 * SORT QUEUE BY arrival time
 */
void swap(void* obj_a, void* obj_b, int size_a, int size_b);
void sort_rec(void* start_obj, int* int_comp, int obj_size, int length);
void sort(void* start_obj, int* int_comp, int obj_size, int length);



#endif