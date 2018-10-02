#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "jobhelper.h"




int main() {
	int seed = time(NULL);
	// comment out srand to fix randomization
	//srand(seed);
	struct Job* jobs = malloc(sizeof(Job) * 10) ;
	for(int i = 0; i < 10; i++) {
		jobs[i].arrival_time = rand() % 100;
		jobs[i].service_time = rand() % 11;
		jobs[i].priority = rand() % 5;
	}
	
	// sort the jobs based on arrival times
	job_sort(jobs, 10, 0);
	printf("Sorted by arrival time:\n");
	print_job_order(jobs, 10);
	
	// sort the jobs based on service times
	job_sort(jobs, 10, 1);
	printf("Sorted by service time:\n");
	print_job_order(jobs, 10);
	
	// sort the jobs based on priority times
	job_sort(jobs, 10, 2);
	printf("Sorted by priority:\n");
	print_job_order(jobs, 10);
	
}
