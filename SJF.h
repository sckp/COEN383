#ifndef SJF_H
#define SJF_H

#include "jobhelper.h"
#include "queue.h"

// function to receive the generated job array from main program
void jobs_SJF(struct Job* jobs, struct Job* finished_Jobs, int numJobs);

// process the jobs
void processJobs_SJF(CPU* cpu, Queue* jobQueue, Job* j, Job* finished, int numJobs);

// copy the job from the queue to the CPU
void moveToCPU_SJF(CPU* c, Queue* q);

// this function removes a job from the CPU and puts it in either
// the job queue or the completed jobs array
void removeFromCPU_SJF(CPU* c, Queue* q, Job* complete);

// this function removes jobs from the queue that haven't start
// when the CPU has passed 100
void removeJobFromQueue_SJF(Queue* q, Job* complete);


#endif
