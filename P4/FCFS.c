#include "helper.h"

void FCFS(List* page_list) {
    Page* temp = page_list->head;
    Page* evict = temp;
    while(temp != NULL) {
        if(temp->first_timestamp < evict->first_timestamp) {
           evict = temp;
        }
        temp = temp->next;
    }
    printf("EVICT PAGE | PID: %d, FREQUENCY: %d, LAST TIMESTAMP: %d\n\n", evict->pid, evict->frequency_num, evict->last_timestamp);
    evict->pid = -1;
    evict->page_num = -1;
    evict->state = NO_MEM;
}
