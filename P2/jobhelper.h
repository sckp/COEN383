#ifndef JOBHELPER_H
#define JOBHELPER_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// create the struct to hold job information
typedef struct Job {
	int pid;
	int arrival_time;
	int service_time;
	int priority;
	int remaining_service_time;
	int start_time;
	int finish_time;
} Job;

// initialize the job struct
void jobInitialization(struct Job* j, int i);

// print the order of the jobs
void print_job(struct Job job);

// swap position of the jobs in the array
void change_position(struct Job* job, int i, int j);

// function to sort the job based on the desired parameter passed
// sort parameters:
// 0 - arrival time
// 1 - service time
// 2 - priority
// 3 - remaining service time
void job_sort(struct Job* job, int size, int sort_param);

// function to get the response time
int response_time(struct Job j);

#endif
