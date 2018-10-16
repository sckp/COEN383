#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"

/**
 * Initialize the customers queue
 */
void make_customers(customer** queue, int length){
    *queue = malloc(sizeof(customer) * length);
    for(int x = 0; x < length; x++){
        (*queue)[x].arrival_time = (rand() % 59) + 1;
    }
    sort(*queue, &((*queue)[0].arrival_time), sizeof(customer), length);
    for(int x = 0; x < length; x++){
        (*queue)[x].customer_id[0] = '0';
        (*queue)[x].customer_id[1] = (x + 1) + '0';
        if(x >= 9){
            (*queue)[x].customer_id[0] = (int) (x + 1) / 10 + '0';
            (*queue)[x].customer_id[1] = ((x + 1) % 10) + '0';

        }
    }
    return;
}

/**
 * Print the customer queue
 */
void print_customers(char* seller_name, customer* list, int length){
      printf("\n+------------- Seller Queue %c%c -------------+\n", seller_name[0], seller_name[1]);
    for(int x = 0; x < length; x++){
        printf("|  customer id: %c%c%c%c  |  arrival time: 0:%02d |\n",
               seller_name[0], seller_name[1], list[x].customer_id[0], list[x].customer_id[1], list[x].arrival_time);
    }
    printf("+---------------------+---------------------+\n");
}


/// -------------------------------------   SORT QUEUE BY arrival time -----------------------------------------///
void swap(void* obj_a, void* obj_b, int size_a, int size_b){
    if(size_a != size_b) return;
    void* tmp = malloc(size_a);
    memcpy(tmp, obj_a, size_a);
    memmove(obj_a, obj_b, size_a);
    memmove(obj_b, tmp, size_a);
    free(tmp);
}

void sort_rec(void* start_obj, int* int_comp, int obj_size, int length){
    //Set pointer to beginning of array
    if(length <= 1) return;
    int inc = 1;
    int pivot = length - 1;
    int p_loc = 0;
    char* beg_objb = (char*) start_obj;
    char* int_compb = (char*) int_comp;

    for(int x = 0; x < length; x++){
        if(pivot == p_loc) break;
        if( *( (int*) (int_compb + (obj_size * pivot)) ) < *( (int*) (int_compb + (obj_size * p_loc) ) ) && pivot  > p_loc){
            swap((beg_objb + (obj_size * pivot)), (beg_objb + (obj_size * p_loc)), obj_size, obj_size);
            swap(&pivot, &p_loc, sizeof(int), sizeof(int));
            inc *= -1;
        }
        else if( *( (int*) (int_compb + (obj_size * pivot)) ) > *( (int*) (int_compb + (obj_size * p_loc) ) ) && pivot < p_loc){
            swap((beg_objb + (obj_size * pivot)), (beg_objb + (obj_size * p_loc)), obj_size, obj_size);
            swap(&pivot, &p_loc, sizeof(int), sizeof(int));
            inc *= -1;
        }
        p_loc += inc;
    }
    sort_rec((beg_objb + (obj_size * pivot)), (int*) (int_compb + (obj_size * pivot)), obj_size, length - pivot);
    sort_rec(beg_objb, (int*) int_compb, obj_size, pivot);
    return;
}

void sort(void* start_obj, int* int_comp, int obj_size, int length){
    //Set pointer to beginning of array
    int inc = 1;
    int pivot = length - 1;
    int p_loc = 0;
    char* beg_objb = (char*) start_obj;
    char* int_compb = (char*) int_comp;


    for(int x = 0; x < length; x++){
        if(pivot == p_loc) break;
        if( *( (int*) (int_compb + (obj_size * pivot)) ) < *( (int*) (int_compb + (obj_size * p_loc) ) ) && pivot  > p_loc){
            swap((beg_objb + (obj_size * pivot)), (beg_objb + (obj_size * p_loc)), obj_size, obj_size);
            swap(&pivot, &p_loc, sizeof(int), sizeof(int));
            inc *= -1;
        }
        else if( *( (int*) (int_compb + (obj_size * pivot)) ) > *( (int*) (int_compb + (obj_size * p_loc) ) ) && pivot < p_loc){
            swap((beg_objb + (obj_size * pivot)), (beg_objb + (obj_size * p_loc)), obj_size, obj_size);
            swap(&pivot, &p_loc, sizeof(int), sizeof(int));
            inc *= -1;
        }
        p_loc += inc;
    }
    //start at pivot
    sort_rec((beg_objb + (obj_size * pivot)), (int*) (int_compb + (obj_size * pivot)), obj_size, length - pivot);
    sort_rec(beg_objb, (int*) int_compb, obj_size, pivot);
    return;
}
/// ------------------------------------- End SORT QUEUE BY arrival time -----------------------------------------///