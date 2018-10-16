#include "page.h"

void FCFS (page * page_list) {
  page * temp = page_list ->head;
  page * evict = temp;
  while(temp) {
    if (temp->arrival_time < evict->arrival_time) {
      evict = temp;
    }
    temp = temp -> next;
  }
  
  //TODO print record
  evict -> pid = -1;
}
