#include "helper.h"

void LRU(List* page_list) {
    Page* temp = page_list->head;
    Page* evict = temp;
    int least = temp->last_timestamp;
    while(temp != NULL) {
        if(temp->last_timestamp < least){
            evict = temp;
            least = temp->last_timestamp;
        }
        temp = temp->next;
    }
    evict->pid = -1;
    evict->page_num = -1;
    evict->state = NO_MEM;
}
