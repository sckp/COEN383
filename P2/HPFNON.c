//P2 HPF non-preemptive

#include "job_helper.h"

#DEFINE QUANTA 100
#DEFINE NUM_JOBS 10
#DEFINE ARRIVAL 1
#DEFINE PRIORITY 2

int HPF(Jobs *job) {
	//SORT JOBS BY ARRIVAL TIME
	job_sort(job, NUM_JOBS, ARRIVAL);

	//FOUR PRIORITY QUEUES OF JOBS
	Jobs priority_queue[4][NUM_JOBS]; 
	
	//TEMP TO GRAB RUNNING JOB
	Jobs *current_job = NULL;
	int current_queue = -1;
	int current_queue_index = 0;
	
	//INDEX OF WAITING JOBS
	int index = 0;
	//INDEX OF EACH PRIORITY QUEUE
	int priority_index[4] = {0};

	//TRACKING WHAT QUANTA JOBS START
	int start_queue[10] = {-1}; 
	//TRACKING LAST TIME JOBS START
	int end_queue[10] = {-1};
	//MARKING WHICH A JOB FINISHES
	int done_queue[10] = {0};

	//QUANTUM IS STATING HERE
	for (int i = 0; i < QUANTA + 50; i ++) {

		//CHECK TO MAKE SURE JOBS ARE IN TIME LIMIT TO BE ADDED TO QUEUE
		if (i < QUANTA) { 
			//ADDING NEW JOBS TO PRIORITY QUEUE
			while (index < NUM_JOBS && job[index].arrival_time <= i) { 
				int new_jprior = job[index].priority - 1;
				//INSERTING JOB TO CORRECT PRIORITY QUEUE
				priority_queue[new_jprior][priority_index[new_jprior]] = jobs[index];
				//UPDATING INDEX/NUMBER OF JOBS IN EACH PRIORITY QUEUE
				priority_index[new_jprior]++;
				//UPDATING INDEX OF WAITING JOBS 
				index++;
			}
		}

		//add aging here

		//CHECK IF A JOB IS RUNNING
		if (current_job == NULL) {
			//CHECK IF IN TIME LIMIT TO START NEW JOB
			if (q >= QUANTA) {
				break;
			}
			//FIND NEXT JOB TO RUN IN PRIORITY QUEUES
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < priority_index[i]; j++) {
					current_queue = i;
					break;
				}
			}

			while (current_queue_index < priority_index[current_queue] && priority_queue[current_queue_index].service_time <= 0) {
				current_queue_index ++;
			}
			current_job = &(priority_queue[current_queue][current_queue_index]);
		}
		//UPDATE START QUEUE
		if (start_queue[current_job->pid] == -1) {
			start_queue[current_job->pid] = i;
		}
		//UPDATE END QUEUE
		end_queue[current_job->pid] = i;

		//UPDATE SERVICE TIME
		current_job->service_time -= 1;

		//NARK PROCESS AS FINISHED AND CLEAR CURRENT JOB
		if current_job->service_time <= 0 {
			done_queue[target_process->pid] = 1; 
			current_job = NULL; 
		}	
	}
}

