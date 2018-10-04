#include "jobhelper.h"

// initialize the job struct
void jobInitialization(struct Job *j, int i) {
	j->pid = i;
	j->arrival_time = rand() % 100;
	j->service_time = rand() % 11;
	j->priority = rand() % 5;
	j->remaining_service_time = j->service_time;
	j->start_time = j->arrival_time;
	j->finish_time = j->arrival_time;	
}

// print the order of the jobs
void print_job(struct Job job) {
	printf("Job Number: %i\n", job.pid);
	printf("Arrival Time: %i\t\tService Time:%i\t\tPriority: %i\n", job.arrival_time, job.service_time, job.priority);
}

// swap position of the jobs in the array
void change_position(struct Job* job, int i, int j) {
	struct Job temp;
	
	temp.pid = job[i].pid;
	temp.arrival_time = job[i].arrival_time;
	temp.service_time = job[i].service_time;
	temp.priority = job[i].priority;
	temp.remaining_service_time = job[i].remaining_service_time;
	temp.start_time = job[i].start_time;
	temp.finish_time = job[i].finish_time;
	
	job[i].pid = job[j].pid;
	job[i].arrival_time = job[j].arrival_time;
	job[i].service_time = job[j].service_time;
	job[i].priority = job[j].priority;
	job[i].remaining_service_time = job[j].remaining_service_time;
	job[i].start_time = job[j].start_time;
	job[i].finish_time = job[j].finish_time;
	
	job[j].pid = temp.pid;
	job[j].arrival_time = temp.arrival_time;
	job[j].service_time = temp.service_time;
	job[j].priority = temp.priority;
	job[j].remaining_service_time = temp.remaining_service_time;
	job[j].start_time = temp.start_time;
	job[j].finish_time = temp.finish_time;
	
}

// function to sort the job based on the desired parameter passed
// sort_param: 0 - arrival time, 1 - service time, 2 - priority
// sort_param: 3 - remaining service time
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
	else if(3 == sort_param) {
		for(int i = 0; i < size; i++) {
			for(int j = i+1; j < size; j++) {
				if(job[i].remaining_service_time > job[j].remaining_service_time) {
					change_position(job, i, j);
				}
			}
		}
	}
}

// function to get the response time
int response_time(struct Job j) {
	return (j.start_time - j.arrival_time);
}
