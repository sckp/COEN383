#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "SJF.h"

// clock variables
int cpu_clock_SJF = 0;

// create an index into the jobs array to use to track which jobs
// have been loaded into the queue and which have finished
int jobIndexSJF = 0;
int finishedIndexSJF = 0;

// struct to hold all of the cpu usuage results
struct cpu_Use {
	int pid;
	int start;
	int end;
};

struct cpu_Use* results;
int result_index = 0;

// function to receive the generated job array from main program
void jobs_SJF(Job* jobs, Job* finished_jobs, int numJobs) {

	// create a job queue
	Queue job_queue;
	// initialize the job queue
	initialize(&job_queue);

	// create an array for all cpu usage results
	results = malloc(sizeof(struct cpu_Use) * 256);
	// initialize all results
	for (int i = 0; i < 256; i++) {
		results[i].pid = -1;
		results[i].start = -1;
		results[i].end = -1;
	}
	result_index = 0;

	// create a CPU instance
	CPU cpu;
	// Initialization of the CPU
	cpu.available = true;

	// sort the jobs based on arrival time
	job_sort(jobs, numJobs, 0);


	processJobs_SJF(&cpu, &job_queue, jobs, finished_jobs, numJobs);
	// sort the completed jobs by their arrival time
	job_sort(finished_jobs, numJobs, 0);

	printf("\n\n Finished Jobs SJF:\n");
	// print jobs based on finish time
	for (int i = 0; i < numJobs; i++) {
		printf(
				"ID: %i\tarrival time: %i   service time: %i\tstart time: %i\tfinish time: %i\n",
				finished_jobs[i].pid, finished_jobs[i].arrival_time,
				finished_jobs[i].service_time, finished_jobs[i].start_time,
				finished_jobs[i].finish_time);
	}

	printf("\n\nCPU Time Table:\n");
	for (int i = 0; i < 256; i++) {
		if (-1 != results[i].pid) {
			//printf("Job ID: %i\tStart: %i\tEnd: %i\n", results[i].pid, results[i].start, results[i].end);
			printf("%i ", results[i].pid);
		}
	}
	printf("\n\n");

	printf("\nThe average response time is: %f\n",
			avg_response_time(finished_jobs, numJobs));
	printf("The average turnaround time is: %f\n",
			avg_turnaround_time(finished_jobs, numJobs));
	printf("The average wait time is: %f\n",
			avg_wait_time(finished_jobs, numJobs));
	throughput_SJF(finished_jobs, numJobs);
}

void processJobs_SJF(CPU* cpu, Queue* jobQueue, Job* jobs, Job* completed,
		int numJobs) {
	// initialize the job struct in the cpu
	cpu->job = malloc(sizeof(Job));

	// loop through all of the jobs
	// Metrics for the loop are if the cpu clock is under 100
	// there is a job on the cpu
	// or if there is a job in the queue
	while ((100 > cpu_clock_SJF) || (!cpu->available) || (!isEmpty(jobQueue))) {
		// check if the cpu clock has reached 100
		if (100 == cpu_clock_SJF) {
			// remove all jobs from the queue that have not started yet
			removeJobFromQueue_SJF(jobQueue, completed);
		}
		// check if a job has arrive and if it should be place into the queue
		// use a while loop to add all jobs that are supposed to arrive
		// incase multiple arrive at the same time
		while ((cpu_clock_SJF == jobs[jobIndexSJF].arrival_time)
				&& (numJobs > jobIndexSJF)) {
			push(jobQueue, jobs[jobIndexSJF]);
			serviceSort(jobQueue);  // added this
			// increment the jobIndexSJF
			jobIndexSJF++;
		}
		// remove a job from the cpu if one is there
		if (!cpu->available) {
			// job has run for its time so remove it from the CPU
			removeFromCPU_SJF(cpu, jobQueue, completed);
		}
		// check if there is a job on the CPU
		if (cpu->available) { // CPU is available
			// check if there is a job on the queue
			if (!isEmpty(jobQueue)) {
				// move the job from the queue to the CPU
				moveToCPU_SJF(cpu, jobQueue);
			}
			// otherwise the CPU remains idle
		}
		// increment the global clock
		cpu_clock_SJF++;
	}
	// free the memory in the CPU struct
	free(cpu->job);
}

// this function moves a job from the queue to the CPU
void moveToCPU_SJF(CPU* c, Queue* q) {
	c->job->pid = q->head->job.pid;
	c->job->arrival_time = q->head->job.arrival_time;
	c->job->service_time = q->head->job.service_time;
	c->job->priority = q->head->job.priority;
	c->job->remaining_service_time = q->head->job.remaining_service_time;
	// check if this is the first time the job is using the CPU
	c->job->start_time = cpu_clock_SJF;
	c->job->finish_time = q->head->job.finish_time;
	// remove the job from the queue
	pop(q);
	// set the cpu to being available
	c->available = false;
}

// this function removes a job from the CPU and puts it in either
// the job queue or the completed jobs array
void removeFromCPU_SJF(CPU* c, Queue* q, Job* complete) {

	if ((256 > result_index) &&((result_index == 0) || (results[result_index-1].pid != c->job->pid ))){
		// set the entry for cpu usage results
		results[result_index].pid = c->job->pid;
		results[result_index].start = cpu_clock_SJF - 1;
		results[result_index].end = cpu_clock_SJF;
		result_index++;
	}

	// decrement the remaining service time for the job
	c->job->remaining_service_time -= 1;
	// check if the job needs to go back into the queue or if it goes in the completed jobs list
	if (1 > c->job->remaining_service_time) {
		// job has finished so it can be replaced in the completed jobs list

		complete[finishedIndexSJF].pid = c->job->pid;
		complete[finishedIndexSJF].arrival_time = c->job->arrival_time;
		complete[finishedIndexSJF].service_time = c->job->service_time;
		complete[finishedIndexSJF].priority = c->job->priority;
		complete[finishedIndexSJF].remaining_service_time =
				c->job->remaining_service_time;
		complete[finishedIndexSJF].start_time = c->job->start_time;
		complete[finishedIndexSJF].finish_time = cpu_clock_SJF;
		// increment the finished index counter
		finishedIndexSJF++;
	}
	else {
		return;
	}
	// set the CPU to being free
	c->available = true;
}

// this function removes jobs from the queue that haven't start
// when the CPU has passed 100
void removeJobFromQueue_SJF(Queue* q, Job* complete) {
	Queue temp;
	initialize(&temp);
	// copy all valid jobs into the temp queue
	while (!isEmpty(q)) {
		if (0 < q->head->job.start_time) {
			push(&temp, q->head->job);
		}
		// remove the job from the queue and place it in the finished array
		// even though it didn't get a chance to run
		else {
			// job has finished so it can be replaced in the completed jobs list
			complete[finishedIndexSJF].pid = q->head->job.pid;
			complete[finishedIndexSJF].arrival_time = q->head->job.arrival_time;
			complete[finishedIndexSJF].service_time = q->head->job.service_time;
			complete[finishedIndexSJF].priority = q->head->job.priority;
			complete[finishedIndexSJF].remaining_service_time =
					q->head->job.remaining_service_time;
			complete[finishedIndexSJF].start_time = q->head->job.start_time;
			complete[finishedIndexSJF].finish_time = cpu_clock_SJF;
			// increment the finished index counter
			finishedIndexSJF++;
		}
		pop(q);
	}
	// reload all jobs that have already started into the job queue
	while (!isEmpty(&temp)) {
		push(q, temp.head->job);
		pop(&temp);
	}
}

// this function calculates the throughput
void throughput_SJF(Job* j, int numJobs) {
	double completedInTime = 0;
	double endTime = 0;
	for(int i = 0; i < numJobs; i++) {
		// check which jobs completed in 100 quanta
		if(-1 != j[i].start_time) {
			completedInTime++;
		}
		if(endTime < j[i].finish_time) {
			endTime = j[i].finish_time;
		}
	}
	// print the throughput
	printf("The throughput rate is: %f\n", (completedInTime / endTime));
}
