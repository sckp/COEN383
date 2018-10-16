#include "page.h" 

void MFU (list * page_list) {
  page* temp = page_list->head;
  page* evict = temp;
  
  int most = temp->LFU_count;
  
  while(temp) {
    if(temp->LFU_count > most) {
      evict = temp;
      most = temp->LFU_count;
    }
    temp = temp->next;
  }

  //TODO PRINT RECORD
  evict->pid = -1;
  evict->page_num = -1;
}
