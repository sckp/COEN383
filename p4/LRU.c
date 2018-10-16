#include "page.h"

void LRU (list * page_list) {
  page* temp = page_list->head;
  page* evict = temp;
  
  int lru_time = temp->last_timestamp;
  
  while (temp) {
    if (temp->last_timestamp < lru_time) {
      evict = temp;
      lru_time - temp->last_timestamp;
    }
    temp = temp->next;
  }
  
  //TODO PRINT RECORD
  evict->pid = -1;
  evict->page_num = -1;
}
