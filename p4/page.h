#ifndef PAGE_H
#define PAGE_H

int clock;

typedef struct {
  int pid; //job 
  int page_num;
  struct page* next;
   
  int LFU_count; //count for LFU and MFU
  int first_timestamp; //first time in memeory
  int last_timestamp; //last time used
} page;

typedef struct {
  page *head;
} list;

typdef struct {
  int pid;
  int size; //number of pages
  int arrival_time;
  int service_duration;
} job;

void FCFS(list *);
void LRU(list *);
void LFU(list *);
void MFU(list *);
void RAN(list *);

#endif
