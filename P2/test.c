#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "jobhelper.h"
#include "queue.h"


int main() {
	int seed = time(NULL);
	// comment out srand to fix randomization
	srand(seed);
	struct Job* jobs = malloc(sizeof(Job) * 10) ;
	for(int i = 0; i < 10; i++) {
		jobs[i].pid = i;
		jobs[i].arrival_time = rand() % 100;
		jobs[i].service_time = rand() % 11;
		jobs[i].priority = rand() % 5;
	}
	
	// sort the jobs based on arrival times
	job_sort(jobs, 10, 0);
	printf("Sorted by arrival time:\n");
	
	struct Queue q_job;
	initialize(&q_job);
	
	// add jobs to the queue
	for(int i = 0; i < 10; i++) {
		push(&q_job, jobs[i]);
	}
	
	printf("\nPrint first element of queue\n");
	print_job(front(&q_job));
	
	printf("\nPrinting queue:\n");
	print_queue(&q_job);
	

}
