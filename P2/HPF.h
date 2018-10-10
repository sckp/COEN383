#ifndef PREHPF_H
#define PREHPF_H

#include "jobhelper.h"
#include "queue.h"

// function to receive the generated job array from main program
void jobs_HPF(Job* jobs, Job* finished_jobs, int numJobs);

// process the jobs
void processJobs_nonHPF(CPU* cpu, Queue* queue, Job* j, Job* finished, int numJobs);

// copy the job from the queue to the CPU
void moveToCPU_nonHPF(CPU* c, Queue* q);

// this function removes a job from the CPU and puts it in either
// the job queue or the completed jobs array
void removeFromCPU_nonHPF(CPU* c, Queue* q, Job* complete, int queueIndex);

// this function removes jobs from the queue that haven't start
// when the CPU has passed 100
void removeJobFromQueue_nonHPF(Queue* q, Job* complete);

void ageProcesses_HPF(Queue *q);

#endif

