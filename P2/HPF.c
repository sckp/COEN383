/*  CODE BY JARRAD NEGHERBON
 *  
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "HPF.h"

//GLOBAL CLOK
int clock_cpu_HPF = 0;

//TIME QUANTUM
const int quanta_nonHPF = 1;

//INDEX INTO JOB ARRAY POINTING TO LOADED
int indexJob = 0;
//INDEX INTO JOB ARRAY POINTING TO FINISHED
int indexFinished = 0;


// struct to hold all of the cpu usuage results
struct cpu_Use {
    int pid;
    int start;
    int end;
};

struct cpu_Use* results;
int result_index;

//GERENATE JOB ARRAY FROM MAIN INPUT
void jobs_HPF(Job *jobs, Job *finished_jobs, int numJobs) {
	//4 PRIORITY JOB QUEUES
	Queue *jobQueues = malloc(sizeof(Queue) *4);

	//INITIALIZE 4 PRIORITY JOB QUEUES
	for (int i = 0; i<4; i++) {
		initialize(&jobQueues[i]);
	}

	CPU cpu;
	cpu.available = true;

	job_sort(jobs, numJobs, 0);

	processJobs_nonHPF(&cpu, jobQueues, jobs, finished_jobs, numJobs);

	job_sort(finished_jobs, numJobs, 4);

	printf("\nFinished Jobs Pre HPF:\n");
	// print jobs based on finish time
	for(int i = 0; i < numJobs; i++) {
		printf("ID: %i\tarrival time: %i   service time: %i\tstart time: %i\tfinish time: %i\n",
			finished_jobs[i].pid, finished_jobs[i].arrival_time, finished_jobs[i].service_time,
			finished_jobs[i].start_time, finished_jobs[i].finish_time);
	}
}


void processJobs_nonHPF(CPU *cpu, Queue *queue, Job *jobs, Job *completed, int numJobs) {		cpu->job = malloc(sizeof(Job));


    while((100 >clock_cpu_HPF) || (!cpu->available) || (!isEmpty(&queue[0])) || (!isEmpty(&queue[1])) || (!isEmpty(&queue[2])) || (!isEmpty(&queue[3]))) {
        if (100 == clock_cpu_HPF) {
            for (int i = 0; i < 4; i++) {
                removeJobFromQueue_nonHPF(&queue[i], completed);
            }
        }
        while ((clock_cpu_HPF == jobs[indexJob].arrival_time) && (numJobs > indexJob)) {
            push(&queue[(jobs[indexJob].priority-1)], jobs[indexJob]);
            indexJob++;
        }

        if(cpu->available) {
            if(!isEmpty(&queue[0])) {
                moveToCPU_nonHPF(cpu, &queue[0]);
            }
            else if(!isEmpty(&queue[1])) {
                moveToCPU_nonHPF(cpu, &queue[1]);
            }
            else if(!isEmpty(&queue[2])) {
                moveToCPU_nonHPF(cpu, &queue[2]);
            }
            else if(!isEmpty(&queue[3])) {
                moveToCPU_nonHPF(cpu, &queue[3]);
            }

        }
        clock_cpu_HPF++;
    }
    free(cpu->job);
}

void moveToCPU_nonHPF(CPU *c, Queue* q) {
    c->job->pid = q->head->job.pid;
    c->job->arrival_time = q->head->job.arrival_time;
    c->job->service_time = q->head->job.service_time;
    c->job->priority = q->head->job.priority;
    c->job->remaining_service_time = q->head->job.remaining_service_time;

    if(1 > q->head->job.start_time) {
        c->job->start_time = clock_cpu_HPF;
    }
    else {
        c->job->start_time = q->head->job.start_time;
    }
    c->job->finish_time = q->head->job.finish_time;
    c->job->age = 0;

    // remove the job from the queue
    pop(q);
        c->available = false;
}

	void removeFromCPU_nonHPF(CPU *c, Queue *q, Job *complete, int queueIndex) {

        ageProcessesHPF(q);

		c->job->remaining_service_time -= quanta_nonHPF;

		if(1 > c->job->remaining_service_time) {
			complete[indexFinished].pid = c->job->pid;
			complete[indexFinished].arrival_time = c->job->arrival_time;
			complete[indexFinished].service_time = c->job->service_time;
			complete[indexFinished].priority = c->job->priority;
			complete[indexFinished].remaining_service_time = c->job->remaining_service_time;
			complete[indexFinished].start_time = c->job->start_time;
			complete[indexFinished].finish_time = clock_cpu_HPF;
			
			indexFinished++;
		}
		c->available = true;
	}

void removeJobFromQueue_nonHPF(Queue *q, Job *complete) {
    Queue temp;
    initialize(&temp);

    while(!isEmpty(q)) {
        if(0 < q->head->job.start_time) {
            push(&temp, q->head->job);
        } else {
            complete[indexFinished].pid = q->head->job.pid;
            complete[indexFinished].arrival_time = q->head->job.arrival_time;
            complete[indexFinished].service_time = q->head->job.service_time;
            complete[indexFinished].priority = q->head->job.priority;
            complete[indexFinished].remaining_service_time = q->head->job.remaining_service_time;
            complete[indexFinished].start_time = q->head->job.start_time;
            complete[indexFinished].finish_time = clock_cpu_HPF;
        
            indexFinished++;
        }
        pop(q);
    }
    while (!isEmpty(&temp)) {
        push(q, temp.head->job);
        pop(&temp);
            
    }
}

//AFTER A PROCESS HAS WAITED FOR 5 QUANTA AT A PRIORITY LEVEL
//BUMP IT UP TO THE NEXT HIGHER LEVEL
void ageProcessesHPF(Queue *q) {
    //NUMBER OF JOBS ON PRIORITY QUEUES
    int jobSize = queue_size(&q[1]) + queue_size(&q[2]) + queue_size(&q[3]);

    //NO JOBS WAITING
    if (1 > jobSize) {
        return;
    }

    //CURRENT JOB
    Job *j = malloc(sizeof(Job) *jobSize);
    int index = 0;

    
    for (int i = 1; i < 4; i++) {
        while(!isEmpty(&q[i])) {
            j[index] = front(&q[i]);

            j[index].age++;
            
            index++;
            pop(&q[i]);
        }
    }
    
    for (int i = 0; i < jobSize; i++) {
        if (6 > j[i].age) {
            j[i].priority--;
            j[i].age = 0;
        }
        push(&q[(j[i].priority-1)], j[i]);
        printf("putting job: %i in queue: %i\n", j[i].pid, j[i].priority-1);
    }
}



