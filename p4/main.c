#include "page.h"

int main (int argc, char *argv[]) {
  
  int clock = 0;                         
  int total_hits = 0;
  int total_misses = 0;
  float hit_miss_ratio = 0;
  
  if (argc != 2) {
    printf("invalid option\n");
    return -1;
  }
  int flg = -1;                      
  if(strcmp(argc[1], "fcfs") == 0) {
    flg = 0; 
  }
  else if(strcmp(argc[1], "lru") == 0) {
    flg = 1; 
  } 
  else if(strcmp(argc[1], "lfu") == 0) {
    flg = 2; 
  }         
  else if(strcmp(argc[1], "mfu") == 0) {
    flg = 3; 
  } 
  else if(strcmp(argc[1], "ran") == 0) {
    flg = 4; 
  }  
  else {
    printf("invalid option\n");
    return -1;
  }
  
  //JOB SIZE OPTIONS
  int *job_size = malloc(sizeof(int)*4);
  job_size[0] = 5;
  job_size[1] = 11;
  job_size[2] = 17;
  job_size[3] = 31;
  
  //FIVE RUNS
  srand(0);
  for(int i = 0; i< 5; i++) {
    //RESET HIT AND MISS
    total_hits = 0;
    total_misses = 0;
  
    List page_list;
    //TODO
    create_list(&page_list);
    //TODO HOW MANY JOBS DO WE NEED 500 = # JOBS
    Job jobs[500];
    
    //TODO 500 = # JOBS
    for(int i = 0; i < 500; i++) {
      jobs[i].pid = i;
      //GIVE JOB A PAGE COUNT FROM OPTIONS
      jobs[i].size = job_size[rand()%4];
      //ARRIVAL WITHIN ONE MINUTE
      jobs[i].arrival_time = rand()%60;
      jobs[i].service_duration = rand()%60;
    }
    
    //TODO SORT THE QUEUE BY ARRIVAL TIME
    
    int queue_index = 0;
    
    //CHECKING FOR NEW JOBS
    for (clock = 0; clock < 60; clock++) {
      while (queue_index < 500 && jobs[queue_index].arrival.time <= clock) {
        //TODO CHECK FOR FREE PAGES AND IF YES BRING JOB INTO MEMORY  
      }
      else {
        //NOT ENOUGH MEMORY TO BRING JOB IN
        break; 
      }
      
    }
    //NEW REQUEST EVERY 100MS BY ALL JOBS IN MEM
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < queue_index; j++) {
        if (jobs[j].service_duration > 0) {
          //TODO CHECK IF PAGE IS ALREADY IN MEMORY AND GET PAGE 
        }
      }
    }
    
    //PAGE IS NOY IN MEMORY YET
    //TODO FIND FREE PAGE OR EVICT A PAGE TO MAKE ROOM
  }
}
