#include "page.h"

void FCFS (Page * page_list) {
  Page * temp = page_list ->head;
  Page * evict = temp;
  while(temp) {
    if (temp->arrival_time < evict->arrival_time) {
      evict = temp;
    }
    temp = temp -> next;
  }
  
  //TODO print record
  evict -> pid = -1;
  evict -> page_num = -1;
}
