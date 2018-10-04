#ifndef JOB_HELPER
#define JOB_HELPER

#include <stdio.h>
#include <stdlib.h>

// create the struct to hold job information
typedef struct Job {
	int pid;
	int arrival_time;
	int service_time;
	int priority;
	int remaining_service_time;
	int start_job_time;
	int finish_job_time;
	
	
} Job;

// print the order of the jobs
void print_job(struct Job job) {
	printf("Job Number: %i\n", job.pid);
	printf("Arrival Time: %i\t\tService Time:%i\t\tPriority: %i\n", job.arrival_time, job.service_time, job.priority);
}

// swap position of the jobs in the array
void change_position(struct Job* job, int i, int j) {
	struct Job temp;
	
	temp.arrival_time = job[i].arrival_time;
	temp.service_time = job[i].service_time;
	temp.priority = job[i].priority;
	
	job[i].arrival_time = job[j].arrival_time;
	job[i].service_time = job[j].service_time;
	job[i].priority = job[j].priority;
	
	job[j].arrival_time = temp.arrival_time;
	job[j].service_time = temp.service_time;
	job[j].priority = temp.priority;
}


// function to sort the job based on the desired parameter passed
// sort_param: 0 - arrival time, 1 - service time, 2 - priority
void job_sort(struct Job* job, int size, int sort_param) {
	// check if we want to sort by arrival time
	if(0 == sort_param) {
		for(int i = 0; i < size; i++) {
			for(int j = i + 1; j < size; j++) {
				if(job[i].arrival_time > job[j].arrival_time) {
					change_position(job, i, j);
				}
			}
		}
	}
	// check if we want to sort by service time
	else if(1 == sort_param) {
		for(int i = 0; i < size; i++) {
			for(int j = i + 1; j < size; j++) {
				if(job[i].service_time > job[j].service_time) {
					change_position(job, i, j);
				}
			}
		}
	}
	// check if we want to sort by priority level
	else if(2 == sort_param) {
		for(int i = 0; i < size; i++) {
			for(int j = i + 1; j < size; j++) {
				if(job[i].priority < job[j].priority) {
					change_position(job, i, j);
				}
			}
		}
	}
}






#endif
