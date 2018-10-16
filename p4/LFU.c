#include "page.h"

void LFU (list * page_list) {
  page* temp = page_list ->head;
  page* evict = temp;
  int least = temp->LFU_count;
  
  while(temp) {
    if (temp->LFU_count < least) {
      evict = temp;
      least = temp->LFU_count;
    }
    temp = temp->next;
  }
  
  //TODO PRINT RECORD
  evict->pid = -1;
  evict->page_num = -1;
}
