#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "preHPF.h"

// create a global clock
int cpu_clock_HPF = 0;
// create a variable to use for the quantum of time
const int quanta_HPF = 1;

// create an index into the jobs array to use to track which jobs
// have been loaded into the queue and which have finished
int jobIndex_HPF = 0;
int finishedIndex_HPF = 0;

// function to receive the generated job array from main program
void jobs_Pre_HPF(Job* jobs, Job* finished_jobs, int numJobs) {
	// initialize the 4 job queues
	Queue* jobQueues = malloc(sizeof(Queue) * 4);
	
	// initialize the 4 job queues
	for(int i = 0; i < 4; i++) {
		initialize(&jobQueues[i]);
	}
	// create a CPU instance
	CPU cpu;
	// Initialization of the CPU
	cpu.available = true;
	// sort the jobs based on arrival time
	job_sort(jobs, numJobs, 0);
	// being processing the jobs
	processJobs_HPF(&cpu, jobQueues, jobs, finished_jobs, numJobs);
	// sort the completed jobs by their arrival time
	job_sort(finished_jobs, numJobs, 2);
	
	printf("\nFinished Jobs Pre HPF:\n");
	// print jobs based on finish time
	for(int i = 0; i < numJobs; i++) {
		printf("ID: %i\tarrival time: %i   service time: %i\tstart time: %i\tfinish time: %i\n",
			finished_jobs[i].pid, finished_jobs[i].arrival_time, finished_jobs[i].service_time,
			finished_jobs[i].start_time, finished_jobs[i].finish_time);
	}
	
}

// begin process of the jobs
void processJobs_HPF(CPU* cpu, Queue* queue, Job* jobs, Job* completed, int numJobs) {
	// initialize the job struct in the cpu
	cpu->job = malloc(sizeof(Job));
	
	// loop through all of the jobs, each loops represents a quantum of time
	// Metrics for the loop are if the cpu clock is under 100
	// there is a job on the cpu
	// or if there is a job in any of the queues
	while((100 > cpu_clock_HPF) || (!cpu->available) || (!isEmpty(&queue[0])) || (!isEmpty(&queue[1]))
		|| (!isEmpty(&queue[2])) || (!isEmpty(&queue[3]))) {
		// check if the cpu clock has reached 100
		if(100 == cpu_clock_HPF) {
			// remove all jobs from all queues that have not started yet
			for(int i = 0; i < 4; i++) {
				removeJobFromQueue_HPF(&queue[i], completed);
			}
		}
		// check if a job has arrive and if it should be place into the queue
		// use a while loop to add all jobs that are supposed to arrive
		// incase multiple arrive at the same time
		while((cpu_clock_HPF == jobs[jobIndex_HPF].arrival_time) && (numJobs > jobIndex_HPF)){
			// place the job into its respective job queue
			push(&queue[(jobs[jobIndex_HPF].priority - 1)], jobs[jobIndex_HPF]);
			// increment the jobIndex
			jobIndex_HPF++;
		}
		// check if there is a job on the CPU
		if(cpu->available) { // CPU is available
			// check if there is a job in the highest priority queue
			if(!isEmpty(&queue[0])) {
				// move the job from the queue to the CPU
				moveToCPU_HPF(cpu, &queue[0]);
			}
			// else check the next priority queue
			else if(!isEmpty(&queue[1])) {
				// move the job from the queue to the CPU
				moveToCPU_HPF(cpu, &queue[1]);
			}
			// else check the next priority queue
			else if(!isEmpty(&queue[2])) {
				// move the job from the queue to the CPU
				moveToCPU_HPF(cpu, &queue[2]);
			}
			// else check the next priority queue
			else if(!isEmpty(&queue[3])) {
				// move the job from the queue to the CPU
				moveToCPU_HPF(cpu, &queue[3]);
			}
			// otherwise the CPU remains idle
		}
		else {  // CPU is not available
			// job has run for its quanta of time so remove it from the CPU
			// check which queue the job on the CPU is supposed to be placed back into
			if(1 == cpu->job->priority) {
				removeFromCPU_HPF(cpu, queue, completed, 0);
			}
			else if(2 == cpu->job->priority) {
				removeFromCPU_HPF(cpu, queue, completed, 1);
			}
			else if(3 == cpu->job->priority) {
				removeFromCPU_HPF(cpu, queue, completed, 2);
			}
			else if(4 == cpu->job->priority) {
				removeFromCPU_HPF(cpu, queue, completed, 3);
			}
		}
		// increment the global clock
		cpu_clock_HPF++;
	}
	// free the memory in the CPU struct
	free(cpu->job);
}

// this function moves a job from the queue to the CPU
void moveToCPU_HPF(CPU* c, Queue* q) {
	c->job->pid = q->head->job.pid;
	c->job->arrival_time = q->head->job.arrival_time;
	c->job->service_time = q->head->job.service_time;
	c->job->priority = q->head->job.priority;
	c->job->remaining_service_time = q->head->job.remaining_service_time;
	// check if this is the first time the job is using the CPU
	if(1 > q->head->job.start_time) {
		c->job->start_time = cpu_clock_HPF;
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
void removeFromCPU_HPF(CPU* c, Queue* q, Job* complete, int queueIndex) {
	// decrement the remaining service time for the job
	c->job->remaining_service_time -= quanta_HPF;
	// check if the job needs to go back into the queue or if it goes in the completed jobs list
	if(1 > c->job->remaining_service_time) {
		// job has finished so it can be replaced in the completed jobs list
		complete[finishedIndex_HPF].pid = c->job->pid;
		complete[finishedIndex_HPF].arrival_time = c->job->arrival_time;
		complete[finishedIndex_HPF].service_time = c->job->service_time;
		complete[finishedIndex_HPF].priority = c->job->priority;
		complete[finishedIndex_HPF].remaining_service_time = c->job->remaining_service_time;
		complete[finishedIndex_HPF].start_time = c->job->start_time;
		complete[finishedIndex_HPF].finish_time = cpu_clock_HPF;
		// increment the finished index counter
		finishedIndex_HPF++;
	}
	// else the job needs to go back in the queue
	else {
		// check all the queues with regards to their priority level and see if there is a higher
		// priority job waiting
		if(0 == queueIndex) {
			// This job is a high priority job see if there are other high priorities waiting
			if(!isEmpty(&q[0])) {
				push(&q[queueIndex], *c->job);
			}
			else {
				return;
			}
		}
		// check next highest priority queue
		else if(1 == queueIndex) {
			// check if there is a higher priority queue with a job or a job with the same priority
			if((!isEmpty(&q[0])) || (!isEmpty(&q[1]))) {
				push(&q[queueIndex], *c->job);
			}
			else {
				return;
			}
		}
		// check 2nd lowest priority queue
		else if(2 == queueIndex) {
			// check if there is a higher priority queue with a job or a job with the same priority
			if((!isEmpty(&q[0])) || (!isEmpty(&q[1])) || (!isEmpty(&q[2]))) {
				push(&q[queueIndex], *c->job);
			}
			else {
				return;
			}
		}
		// check lowest priority queue
		else if(3 == queueIndex) {
			// check if there is a higher priority queue with a job or a job with the same priority
			if((!isEmpty(&q[0])) || (!isEmpty(&q[1])) || (!isEmpty(&q[2])) || (!isEmpty(&q[3]))) {
				push(&q[queueIndex], *c->job);
			}
			else {
				return;
			}
		}
	}
	// set the CPU to being free
	c->available = true;
}

// this function removes jobs from the queue that haven't start
// when the CPU has passed 100
void removeJobFromQueue_HPF(Queue* q, Job* complete) {
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
			complete[finishedIndex_HPF].pid = q->head->job.pid;
			complete[finishedIndex_HPF].arrival_time = q->head->job.arrival_time;
			complete[finishedIndex_HPF].service_time = q->head->job.service_time;
			complete[finishedIndex_HPF].priority = q->head->job.priority;
			complete[finishedIndex_HPF].remaining_service_time = q->head->job.remaining_service_time;
			complete[finishedIndex_HPF].start_time = q->head->job.start_time;
			complete[finishedIndex_HPF].finish_time = cpu_clock_HPF;
			// increment the finished index counter
			finishedIndex_HPF++;
		}
		pop(q);
	}
	// reload all jobs that have already started into the job queue
	while(!isEmpty(&temp)) {
		push(q, temp.head->job);
		pop(&temp);
	}
}

