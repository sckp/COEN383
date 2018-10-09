#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "jobhelper.h"
#include "queue.h"
#include "preHPF.h"

// header for round robin scheduler
#include "roundrobin.h"


int main() {
	int seed = time(NULL);
	// comment out srand to fix randomization
	srand(seed);
	
	const int number_of_jobs = 30;
	
	struct Job* jobs = malloc(sizeof(Job) * number_of_jobs);
	for(int i = 0; i < number_of_jobs; i++) {
		jobInitialization(&jobs[i], i);
	}
	
		
	// Round Robin Scheduling
	struct Job* RR_jobs = malloc(sizeof(Job) * number_of_jobs);
	jobs_Round_Robin(jobs, RR_jobs, number_of_jobs);
	
	// Preemptive HPF Scheduling
	struct Job* preHPF_jobs = malloc(sizeof(Job) * number_of_jobs);
	jobs_Pre_HPF(jobs, preHPF_jobs, number_of_jobs);
	
    //NON-Preemptive HPF Scheduling
    struct Job* HPF_jobs = malloc(sizeof(Job) * number_of_jobs);
    jobs_HPF(jobs, HPF_jobs, number_of_jobs);
}

