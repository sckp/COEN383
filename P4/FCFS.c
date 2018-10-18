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
 
    evict->pid = -1;
    evict->page_num = -1;
    evict->state = NO_MEM;
}
