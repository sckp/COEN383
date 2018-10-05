#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "jobhelper.h"
#include "queue.h"

// header for round robin scheduler
#include "roundrobin.h"


int main() {
	int seed = time(NULL);
	// comment out srand to fix randomization
	//srand(seed);
	
	const int number_of_jobs = 20;
	
	struct Job* jobs = malloc(sizeof(Job) * number_of_jobs);
	for(int i = 0; i < number_of_jobs; i++) {
		jobInitialization(&jobs[i], i);
	}
	
		
	// Round Robin Scheduling
	// Create array for results of round robin
	struct Job* RR_jobs = malloc(sizeof(Job) * number_of_jobs);
	
	jobs_Round_Robin(jobs, RR_jobs, number_of_jobs);

}

