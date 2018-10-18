#include "helper.h"

void MFU(List* page_list) {
    Page* temp = page_list->head;
    Page* evict = temp;
    int most = temp->frequency_num;
    while(temp != NULL) {
        if(temp->frequency_num > most){
           evict = temp;
            most = temp->frequency_num;
        }
        temp = temp->next;
    }
   
    evict->pid = -1;
    evict->page_num = -1;
    evict->state = NO_MEM;
}
