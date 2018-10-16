#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "seller.c"
#include "customer.c"
#include "global.h"


struct itimerval sellerTimer;

void timerHandler(int signal){
    print_stats();
}

void wake_up(){
    pthread_mutex_lock(&seat_access);
    start = true;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&seat_access);
}

int main(int argc, char *argv[]){

    NUM_OF_CUSTOMERS = 15; // default customer size

    if(argc >= 2) {
        NUM_OF_CUSTOMERS= atoi(argv[1]);
        printf("\nNumber of customers : %d\n\n", NUM_OF_CUSTOMERS);
    }

    // Initilize total sell counts for L, M and H sellers
    totalL_sold = 0;
    totalM_sold = 0;
    totalH_sold = 0;

    //Initilization of variables
    int seed = 1994;
    srand(seed);
    start = false;

    // Set timer to exit at 60 minutes
    sellerTimer.it_value.tv_sec = MAX_MINUTES;
    setitimer(ITIMER_REAL, &sellerTimer, NULL);
    time(&startTime);

    time_t currentTime; // type calender time
    time(&currentTime);
    int sec = (int) difftime(currentTime, startTime);

    seat_map = malloc(TOTAL_SEATS * (sizeof(char) * 4)); //100 seats can store 4 each seat
    memset(seat_map, 45, TOTAL_SEATS * (sizeof(char) * 4));
    seller* h_sellers = malloc(sizeof(seller) * NUM_OF_H_SELLERS);//create h_sellers
    seller* m_sellers = malloc(sizeof(seller) * NUM_OF_M_SELLERS);//create m_sellers
    seller* l_sellers = malloc(sizeof(seller) * NUM_OF_L_SELLERS);//create l_sellers
    filled_seats = 0;

    //Setting and threads
    seat_access = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    cond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    pthread_t h_seller_thread;
    pthread_t m_sellers_thread[3];
    pthread_t l_sellers_thread[6];


    // Populate high seller
    h_sellers[0].name[0] = 'H';
    h_sellers[0].name[1] = '0';

    make_customers(&(h_sellers[0].start_queue), NUM_OF_CUSTOMERS);
    print_customers(h_sellers[0].name, h_sellers[0].start_queue, NUM_OF_CUSTOMERS);

    //Populate all medium sellers
    for(int x = 0; x < NUM_OF_M_SELLERS; x++){
        m_sellers[x].name[0] = 'M';
        m_sellers[x].name[1] = (char) (x + '0');

        make_customers(&(m_sellers[x].start_queue), NUM_OF_CUSTOMERS);
        print_customers(m_sellers[x].name, m_sellers[x].start_queue, NUM_OF_CUSTOMERS);
    }

    // Populate all low sellers
    for(int x = 0; x < NUM_OF_L_SELLERS; x++){
        l_sellers[x].name[0] = 'L';
        l_sellers[x].name[1] = x + '0';

        make_customers(&(l_sellers[x].start_queue), NUM_OF_CUSTOMERS);
        print_customers(l_sellers[x].name, l_sellers[x].start_queue, NUM_OF_CUSTOMERS);
    }

    printf("\n\nTimer- %1d:%02d\n", (int) (sec / 60), (int) sec % 60); // Print initial time stamp

    // Create seller H thread
    pthread_create(&h_seller_thread, NULL, sell_seats, h_sellers);

    // Create seller M threads
    for(int x = 0; x < NUM_OF_M_SELLERS; x++){
        // arguments(structure, NULL, function that runs in thread, argument to sell_seats function)
        pthread_create(&(m_sellers_thread[x]), NULL, sell_seats, (void*) &m_sellers[x]);
    }
    // Create seller L threads
    for(int x = 0; x < NUM_OF_L_SELLERS; x++){
        pthread_create(&(l_sellers_thread[x]), NULL, sell_seats, (void*) &l_sellers[x]);
    }

    wake_up();
    signal(SIGALRM, timerHandler); // Set signal to listen for when time hits 60 seconds, then call timerHandler function

    //join all threads
    pthread_join(h_seller_thread, NULL);

    for(int x = 0; x < NUM_OF_L_SELLERS; x++){
        pthread_join(l_sellers_thread[x], NULL);
    }

    for(int x = 0; x < NUM_OF_M_SELLERS; x++){
        pthread_join(m_sellers_thread[x], NULL);
    }

    return 0;
}