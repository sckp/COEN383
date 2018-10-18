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
    for (int i=0; i < 500; i++){
        if (jobs[i].state != READY){
            printf("PID: %d, # of Pages: %d, IO: %d, Arrival Time: %d, Service Duration: %d, Current Page: %d, Current State: %s\n",
         jobs[i].pid, jobs[i].page_num, jobs[i].io_num, jobs[i].arrival_time, jobs[i].service_duration, jobs[i].page_now, jobs[i].state );
         }
    }
}
