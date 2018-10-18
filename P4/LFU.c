#include "helper.h"

void LFU(List* page_list) {
    Page* temp = page_list->head;
    Page* evict = temp;
    int least = temp->frequency_num;
    while(temp != NULL) {
        if(temp->frequency_num < least){
            evict = temp;
            least = temp->frequency_num;
        }
        temp = temp->next;
    }
    evict->pid = -1;
    evict->page_num = -1;
    evict->state = NO_MEM;
}
