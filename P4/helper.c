#include "helper.h"

void new_list(List* page_list) {
    page_list->head = (Page *)malloc(sizeof(Page));
    Page* temp = page_list->head;
    for (int i = 0; i < 100; i++) {
        temp->pid = -1;
        temp->page_num = -1;
        temp->state = NO_MEM;
        if(i < 99) {
            temp->next = (Page *)malloc(sizeof(Page));
            temp = temp->next;
        } else {
            temp->next = NULL;
        }
    }
}

int in_memory(List* page_list, int pid, int page_num) {
    Page* temp = page_list->head;
    while(temp != NULL) {
        if(temp ->pid == pid) {
            if (temp->page_num == page_num) {
                return 1;
            }
        }
        temp = temp->next;
    }
    return 0;
}

Page* find_free (List* page_list) {
    Page* temp = page_list->head;
    while(temp != NULL) {
        if( temp->pid == -1 ) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

int free_memory(List* page_list, int pid) {
    Page* temp = page_list->head;
    int num = 0;
    while(temp != NULL) {
        if(temp->pid == pid) {
            temp->pid = -1;
            temp->page_num = -1;
            temp->state = NO_MEM;
            num++;
        }
        temp = temp->next;
    }
    return num;
}

void print_jobs(Job* jobs){
    for (int i=0; i < JOB_NUMBER; i++){
        if (jobs[i].state != READY){
            printf("PID: %d, # of Pages: %d, IO: %d, Arrival Time: %d, Service Duration: %d, Current Page: %d\n",
         jobs[i].pid, jobs[i].page_num, jobs[i].io_num, jobs[i].arrival_time, jobs[i].service_duration, jobs[i].page_now );
         }
    }
}

int arrival_times(const void *a, const void *b) {
    return ((Job*)a)->arrival_time - ((Job*)b)->arrival_time;
}

void job_sort(Job * jobs, int size) {
//    for(int i = 0; i < size; i++) {
//        for(int j = i + 1; j < size; j++) {
//            if(jobs[i].arrival_time > jobs[j].arrival_time) {
//                change_position(jobs, i, j);
//            }
//        }
//    }
    qsort(jobs, size, sizeof(Job), arrival_times);
}
          

void change_position (Job *jobs, int i, int j) {
    void *temp = NULL;
    
    ((Job*)temp)->pid = jobs[i].pid;
    ((Job*)temp)->page_num = jobs[i].page_num;
    ((Job*)temp)->io_num = jobs[i].io_num;
    ((Job*)temp)->arrival_time = jobs[i].arrival_time;
    ((Job*)temp)->page_now = jobs[i].page_now;
    ((Job*)temp)->service_duration = jobs[i].service_duration;
    ((Job*)temp)->state = jobs[i].state;
    
    jobs[i].pid = jobs[j].pid;
    jobs[i].page_num = jobs[j].page_num;
    jobs[i].io_num = jobs[j].io_num;
    jobs[i].arrival_time = jobs[j].arrival_time;
    jobs[i].page_now = jobs[j].page_now;
    jobs[i].service_duration = jobs[j].service_duration;
    jobs[i].state = jobs[j].state;
    
    jobs[j].pid = ((Job*)temp)->pid;
    jobs[j].page_num = ((Job*)temp)->page_num;
    jobs[j].io_num = ((Job*)temp)->io_num;
    jobs[j].arrival_time = ((Job*)temp)->arrival_time;
    jobs[j].page_now = ((Job*)temp)->page_now;
    jobs[j].service_duration = ((Job*)temp)->service_duration;
    jobs[j].state = ((Job*)temp)->state;
}
