#include "shared.h"
#include <stdlib.h>
#include <time.h>

int main() {
    processes *p1;
    generate_processes(p1);
    output_processes(p1);
    return 0;
}

int generate_processes(processes *proc) {
    int seed = time(NULL);
    srand(seed);
    
    for (int i = 0; i < 10; i++) { //generate 10 processes
        proc[i].pid = i;
        
        proc[i].arrival_time = rand() % 100; //will return num between 0 and 99
        proc[i].service_time = rand() % 11; //will return num between 0 and 10
        if (proc[i].service_time == 0) {
            proc[i].service_time += 1;
        }
        proc[i].remaining_service = proc[i].service_time; //all time remains
        proc[i].priority = rand () % 5; //priority between 1 and 4
        if (proc[i].priority == 0) {
            proc[i].priority += 1;
        }
        
    }
    return 0;
}

int compare_arrival(const processes *p1, const processes *p2) {
    if (p1->arrival_time >= p2->arrival_time) {
        return 1;
    }
    else return -1;
}

int compare_service(const processes *p1, const processes *p2) {
    if (p1->service_time >= p2->service_time) {
        return 1;
    }
    else return -1;
}
int compare_remaining_service(const processes *p1, const processes *p2) {
    if (p1->remaining_service >= p2->remaining_service) {
        return 1;
    }
    else return -1;
}

int compare_priority(const processes *p1, const processes *p2) {
    if (p1->priority >= p2->priority) {
        return 1;
    }
    else return -1;
}

int output_processes(processes *proc) {
    for (int i = 0; i < 10; i++) {
        printf(" %d  %d  %d  %d  %d  \n", proc[i].pid, proc[i].arrival_time, proc[i].service_time, proc[i].remaining_service, proc[i].priority);
    }
    return 0;
}
