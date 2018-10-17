#include "page.h"

void LFU (List * page_list) {
  Page* temp = page_list ->head;
  Page* evict = temp;
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
