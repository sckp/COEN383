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
		jobInitialization(&jobs[i], i);
	}
	
	// sort the jobs based on arrival times
	//job_sort(jobs, 10, 0);
	printf("Sorted by arrival time:\n");
	
	struct Queue q_job;
	initialize(&q_job);
	
	// add jobs to the queue
	for(int i = 0; i < 10; i++) {
		push(&q_job, jobs[i]);
	}
		
	printf("\nPrinting based on priority sorted queue:\n");
	prioritySort(&q_job);
	print_queue(&q_job);
	
	printf("\nPrinting based on service time sorted queue:\n");
	serviceSort(&q_job);
	print_queue(&q_job);
	
	
	printf("\nPrinting based on shortest remaining time sorted queue:\n");
	serviceSort(&q_job);
	print_queue(&q_job);

}
