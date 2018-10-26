#include "helper.h"

void RAN(List* page_list) {
    Page *temp = page_list->head;
    Page *evict = temp;
    
    int evict_num = 0;
    int ran_num = rand()%100;
    
    while (temp != NULL) {
        if (evict_num < ran_num && temp->pid >= 0 ) {
            evict = temp;
        }
        temp = temp->next;
        evict_num++;
    }
    
    printf("EVICT PAGE | PID: %d, FREQUENCY: %d, LAST TIMESTAMP: %d\n\n", evict->pid, evict->frequency_num, evict->last_timestamp);
    evict->pid = -1;
    evict->page_num = -1;
    evict->state = NO_MEM;
}

