#ifndef helper_H
#define helper_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int clock;
int *page_count_opts;

enum job_state{
    RUNNING,
    WAITING,
    DONE,
    READY,
};

enum page_state{
    LOADING,
    IN_MEM,
    NO_MEM,
};

typedef struct {
    int pid;
    int page_num;
    int io_num;
    int arrival_time;
    int page_now;
    int service_duration;
    enum job_state state;
} Job;

typedef struct{
    int pid;
    int page_num;
    struct Page* next;
    int first_timestamp;
    int last_timestamp;
    int frequency_num;
    enum page_state state;
} Page;

typedef struct {
    Page* head;
} List;


void new_list(List* page_list);

void FCFS(List* page_list);
void LRU(List* page_list);
void LFU(List* page_list);
void MFU(List* page_list);
void RAN(List* page_list);

int in_memory(List* page_list, int pid, int page_num);
int free_memory(List* page_list, int pid);
Page* find_free(List* page_list);
void print_jobs(Job* jobs);

#endif
