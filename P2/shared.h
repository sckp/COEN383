#ifndef shared_h
#define shared_h

#include <stdio.h>

typedef struct {
    int pid;
    int arrival_time;
    int service_time;
    int remaining_service;
    int priority;
} processes;

int generate_processes(processes *);
int compare_arrival(const processes *, const processes *);
int compare_service(const processes *, const processes *);
int compare_remaining_service(const processes *, const processes *);
int compare_priority(const processes *, const processes *);
int output_processes(processes *);

#endif /* shared_h */
