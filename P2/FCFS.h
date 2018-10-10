#ifndef FCFS_H
#define FCFS_H

#include "jobhelper.h"


// function to receive the generated job array from main program and process it, as well as return throughput
double jobs_FCFS(struct Job* jobs, struct Job* finished_Jobs, int numJobs);

#endif
