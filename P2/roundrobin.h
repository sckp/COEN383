#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include "jobhelper.h"
#include "queue.h"

// function to receive the generated job array from main program
void jobs_Round_Robin(Job* jobs, Job* finished_Jobs, int numJobs);

// process the jobs
void processJobs(CPU* cpu, Queue* jobQueue, Job* j, Job* finished, int numJobs);

// copy the job from the queue to the CPU
void moveToCPU(CPU* c, Queue* q);

// this function removes a job from the CPU and puts it in either
// the job queue or the completed jobs array
void removeFromCPU(CPU* c, Queue* q, Job* complete);

// this function removes jobs from the queue that haven't start
// when the CPU has passed 100
void removeJobFromQueue(Queue* q, Job* complete);

// this function calculates the throughput for 100 quanta
void throughput_RR(Job* j, int numJobs);

#endif
