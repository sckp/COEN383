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
} Page;

typedef struct {
  page *head;
} List;

typdef struct {
  int pid;
  int size; //number of pages
  int arrival_time;
  int service_duration;
} Job;

void FCFS(List *); //first come first serve
void LRU(List *); //least recently used
void LFU(List *); //least frequently used
void MFU(List *); //most frequently used
void RAN(List *); //random

void create_list(List *); //initialize page list
void print_list(List *); //print page list
void print_job(Job * ); //print job queue

#endif
