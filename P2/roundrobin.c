#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "roundrobin.h"

// create a global clock
int cpu_clock = 0;
// create a variable to use for the quantum of time
const int quanta = 1;

// create an index into the jobs array to use to track which jobs
// have been loaded into the queue and which have finished
int jobIndex = 0;
int finishedIndex = 0;

// function to receive the generated job array from main program
void jobs_Round_Robin(Job* jobs, Job* finished_jobs, int numJobs) {
	// create a job queue
	Queue job_queue;
	// initialize the job queue
	initialize(&job_queue);
	// create a CPU instance
	CPU cpu;
	// Initialization of the CPU
	cpu.available = true;
	
	Job* test = malloc(sizeof(Job) * 5);
	for(int i = 0; i < 5; i++) {
		test[i].pid = i;
		test[i].arrival_time = i;
		test[i].service_time = i+1;
		test[i].remaining_service_time = test[i].service_time;
		test[i].start_time = -1;
		test[i].finish_time = -1;
		
	}
	
	
	// sort the jobs based on arrival time
	job_sort(jobs, numJobs, 0);
	
	printf("Jobs to process:\n");
	// print jobs based on finish time
	for(int i = 0; i < numJobs; i++) {
		printf("ID: %i\tarrival time: %i   service time: %i\tstart time: %i\tfinish time: %i\n",
			jobs[i].pid, jobs[i].arrival_time, jobs[i].service_time,
			jobs[i].start_time, jobs[i].finish_time);
	}
	printf("\n\n");
	
	// being processing the jobs
	processJobs(&cpu, &job_queue, jobs, finished_jobs, numJobs);
	
	
	job_sort(finished_jobs, numJobs, 0);
	//job_sort(finished_jobs, 5, 0);
	
	printf("Finished Jobs:\n");
	// print jobs based on finish time
	for(int i = 0; i < numJobs; i++) {
		printf("ID: %i\tarrival time: %i   service time: %i\tstart time: %i\tfinish time: %i\n",
			finished_jobs[i].pid, finished_jobs[i].arrival_time, finished_jobs[i].service_time,
			finished_jobs[i].start_time, finished_jobs[i].finish_time);
	}
	
}

// begin process of the jobs
void processJobs(CPU* cpu, Queue* jobQueue, Job* jobs, Job* completed, int numJobs) {
	// initialize the job struct in the cpu
	cpu->job = malloc(sizeof(Job));
	
	// loop through all of the jobs, each loops represents a quantum of time
	// Metrics for the loop are if the cpu clock is under 100
	// there is a job on the cpu
	// or if there is a job in the queue
	while((100 > cpu_clock) || (!cpu->available) || (!isEmpty(jobQueue))) {
		// check if the cpu clock has reached 100
		if(100 == cpu_clock) {
			// remove all jobs from the queue that have not started yet
			removeJobFromQueue(jobQueue, completed);
		}
		// check if a job has arrive and if it should be place into the queue
		// use a while loop to add all jobs that are supposed to arrive
		// incase multiple arrive at the same time
		while((cpu_clock == jobs[jobIndex].arrival_time) && (numJobs > jobIndex)){
			push(jobQueue, jobs[jobIndex]);
			// increment the jobIndex
			jobIndex++;
		}
		// check if there is a job on the CPU
		if(cpu->available) { // CPU is available
			// check if there is a job on the queue
			if(!isEmpty(jobQueue)) {
				// move the job from the queue to the CPU
				moveToCPU(cpu, jobQueue);
			}
			// otherwise the CPU remains idle
		}
		else {  // CPU is not available
			// job has run for its quanta of time so remove it from the CPU
			removeFromCPU(cpu, jobQueue, completed);
		}
		// increment the global clock
		cpu_clock ++;
	}
	// free the memory in the CPU struct
	free(cpu->job);
}

// this function moves a job from the queue to the CPU
void moveToCPU(CPU* c, Queue* q) {
	c->job->pid = q->head->job.pid;
	c->job->arrival_time = q->head->job.arrival_time;
	c->job->service_time = q->head->job.service_time;
	c->job->priority = q->head->job.priority;
	c->job->remaining_service_time = q->head->job.remaining_service_time;
	// check if this is the first time the job is using the CPU
	if(1 > q->head->job.start_time) {
		c->job->start_time = cpu_clock;
	}
	else {
		c->job->start_time = q->head->job.start_time;
	}
	c->job->finish_time = q->head->job.finish_time;
	// remove the job from the queue
	pop(q);
	// set the cpu to being available
	c->available = false;
}

// this function removes a job from the CPU and puts it in either
// the job queue or the completed jobs array
void removeFromCPU(CPU* c, Queue* q, Job* complete) {
	// decrement the remaining service time for the job
	c->job->remaining_service_time -= quanta;
	// check if the job needs to go back into the queue or if it goes in the completed jobs list
	if(1 > c->job->remaining_service_time) {
		// job has finished so it can be replaced in the completed jobs list
		complete[finishedIndex].pid = c->job->pid;
		complete[finishedIndex].arrival_time = c->job->arrival_time;
		complete[finishedIndex].service_time = c->job->service_time;
		complete[finishedIndex].priority = c->job->priority;
		complete[finishedIndex].remaining_service_time = c->job->remaining_service_time;
		complete[finishedIndex].start_time = c->job->start_time;
		complete[finishedIndex].finish_time = cpu_clock;
		// increment the finished index counter
		finishedIndex++;
	}
	// else the job needs to go back in the queue
	else {
		// only remove the job from the CPU if there is a job waiting
		if(!isEmpty(q)) {
			push(q, *c->job);
		}
		else {
			return;
		}
	}
	// set the CPU to being free
	c->available = true;
}

// this function removes jobs from the queue that haven't start
// when the CPU has passed 100
void removeJobFromQueue(Queue* q, Job* complete) {
	Queue temp;
	initialize(&temp);
	// copy all valid jobs into the temp queue
	while(!isEmpty(q)) {
		if(0 < q->head->job.start_time) {
			push(&temp, q->head->job);
		}
		// remove the job from the queue and place it in the finished array
		// even though it didn't get a chance to run
		else {
			// job has finished so it can be replaced in the completed jobs list
			complete[finishedIndex].pid = q->head->job.pid;
			complete[finishedIndex].arrival_time = q->head->job.arrival_time;
			complete[finishedIndex].service_time = q->head->job.service_time;
			complete[finishedIndex].priority = q->head->job.priority;
			complete[finishedIndex].remaining_service_time = q->head->job.remaining_service_time;
			complete[finishedIndex].start_time = q->head->job.start_time;
			complete[finishedIndex].finish_time = cpu_clock;
			// increment the finished index counter
			finishedIndex++;
		}
		pop(q);
	}
	// reload all jobs that have already started into the job queue
	while(!isEmpty(&temp)) {
		push(q, temp.head->job);
		pop(&temp);
	}
}
