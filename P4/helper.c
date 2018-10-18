#include "helper.h"

void new_list(List* page_list) {
    page_list->head = malloc(sizeof(Page));
    Page* temp = page_list->head;
    for (int i = 0; i < 100; i++) {
        temp->pid = -1;
        temp->page_num = -1;
        temp->state = NO_MEM;
        if(i < 99) {
            temp->next = malloc(sizeof(Page));
            temp = temp->next;
        } else {
            temp->next = NULL;
        }
    }
}

void print_list(List* page_list) {
    Page* temp = page_list->head;

}

int is_free(page_list* pl, int num) {
    
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

int next_page(int page_now, int max_page) {
    
}

page* get_page_from_pid(page_list* pl,int pid,int page_no) {
    page* it = pl->head;
    while(it) {
        if(it->pid == pid && it->page_no == page_no) return it;
        it = it->next;
    }
    return NULL;
}

int compare_arrival_times(const void* a,const void* b) {
    return ((process*)a)->arrival_time - ((process*)b)->arrival_time;
}

void print_jobs(Jobs* jobs){
    for (int i=0; i < 500; i++){
        if (jobs[i].state != READY){
            printf("PID: %d, # of Pages: %d, IO: %d, Arrival Time: %d, Service Duration: %d, Current Page: %d, Current State: %s\n",
         job[i].pid, job[i].page_num, job[i].io_num, job[i].arrival_time, job[i].service_duration, job[i].page_now, job[i].state, );
         }
    }
}
