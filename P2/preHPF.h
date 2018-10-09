#ifndef PREHPF_H
#define PREHPF_H

#include "jobhelper.h"
#include "queue.h"

// function to receive the generated job array from main program
void jobs_Pre_HPF(Job* jobs, Job* finished_jobs, int numJobs);

// process the jobs
void processJobs_HPF(CPU* cpu, Queue* queue, Job* j, Job* finished, int numJobs);

// copy the job from the queue to the CPU
void moveToCPU_HPF(CPU* c, Queue* q);

// this function removes a job from the CPU and puts it in either
// the job queue or the completed jobs array
void removeFromCPU_HPF(CPU* c, Queue* q, Job* complete, int queueIndex);

// this function removes jobs from the queue that haven't start
// when the CPU has passed 100
void removeJobFromQueue_HPF(Queue* q, Job* complete);

// this function will increment the ages of all the waiting processes
void ageProcesses(Queue* q);

#endif

