#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "HPF.h"

// create a global clock
int clock_cpu_HPF = 0;
// create a variable to use for the quantum of time
const int quanta_nonHPF = 1;

// create an index into the jobs array to use to track which jobs
// have been loaded into the queue and which have finished
int indexJob = 0;
int indexFinished = 0;

// struct to hold all of the cpu usuage results_Queue
struct cpu_Use {
    int pid;
    int start;
    int end;
};

struct cpu_Use* results_Queue;
int indexResult;

//RECEIVE ARRAY OF JOBS
void jobs_HPF(Job* jobs, Job* finished_jobs, int numJobs) {
    //INITIALIZE 4 PRIORITY QUEUES
    Queue* jobQueues = malloc(sizeof(Queue) * 4);
    //INITIALIZE 4 JOB QUEUES
    for(int i = 0; i < 4; i++) {
        initialize(&jobQueues[i]);
    }
    
    //QUEUE OF CPU USAGE
    results_Queue = malloc(sizeof(struct cpu_Use) * 256);
    //DEFAULT ALL VALUES TO -1
    for(int i = 0; i < 256; i++) {
        results_Queue[i].pid = -1;
        results_Queue[i].start = -1;
        results_Queue[i].end = -1;
    }
    indexResult = 0;
    
    
    CPU cpu;
    //CPU IS AVAILABLE (NO JOBS RUNNING)
    cpu.available = true;
    //SORT JOBS BASED OFF ARRIVAL TIME
    job_sort(jobs, numJobs, 0);
    
    //PROCESS JOBS
    processJobs_nonHPF(&cpu, jobQueues, jobs, finished_jobs, numJobs);
    //?
    job_sort(finished_jobs, numJobs, 4);
    
    printf("\nFinished Jobs Pre HPF:\n");
    //PRINT JOBS BASED OFF FINISH TIME
    for(int i = 0; i < numJobs; i++) {
        printf("ID: %i\tarrival time: %i   service time: %i\tstart time: %i\tfinish time: %i\n",
               finished_jobs[i].pid, finished_jobs[i].arrival_time, finished_jobs[i].service_time,
               finished_jobs[i].start_time, finished_jobs[i].finish_time);
    }
    
    printf("\n\nCPU Time Table:\n");
    for(int i = 0; i < 256; i++) {
        if(-1 != results_Queue[i].pid) {
            //printf("Job ID: %i\tStart: %i\tEnd: %i\n", results_Queue[i].pid, results_Queue[i].start, results_Queue[i].end);
            printf("%i ", results_Queue[i].pid);
        }
    }
    printf("\n\n");
    
    printf("\nThe average response time is: %f\n", avg_response_time(finished_jobs, numJobs));
    printf("The average turnaround time is: %f\n", avg_turnaround_time(finished_jobs, numJobs));
    printf("The average wait time is: %f\n", avg_wait_time(finished_jobs, numJobs));
    
    //FREE DYNAMIC MEMORY
    free(results_Queue);
}

//PROCESS THE JOBS
void processJobs_nonHPF(CPU* cpu, Queue* queue, Job* jobs, Job* completed, int numJobs) {
    
    cpu->job = malloc(sizeof(Job));
    
    // loop through all of the jobs, each loops represents a quantum of time
    // Metrics for the loop are if the cpu clock is under 100
    // there is a job on the cpu
    // or if there is a job in any of the queues
    while((100 > clock_cpu_HPF) || (!cpu->available) || (!isEmpty(&queue[0])) || (!isEmpty(&queue[1]))
          || (!isEmpty(&queue[2])) || (!isEmpty(&queue[3]))) {
        // check if the cpu clock has reached 100
        if(100 == clock_cpu_HPF) {
            // remove all jobs from all queues that have not started yet
            for(int i = 0; i < 4; i++) {
                removeJobFromQueue_nonHPF(&queue[i], completed);
            }
        }
        // check if a job has arrive and if it should be place into the queue
        // use a while loop to add all jobs that are supposed to arrive
        // incase multiple arrive at the same time
        while((clock_cpu_HPF == jobs[indexJob].arrival_time) && (numJobs > indexJob)){
            // place the job into its respective job queue
            push(&queue[(jobs[indexJob].priority - 1)], jobs[indexJob]);
            // increment the jobIndex
            indexJob++;
        }
        
        // check if the CPU is not available
        if(!cpu->available && 1 > jobs[indexJob].remaining_service_time) {  // CPU is not available
            // job has run for its quanta of time so remove it from the CPU
            // check which queue the job on the CPU is supposed to be placed back into
            if(1 == cpu->job->priority) {
                removeFromCPU_nonHPF(cpu, queue, completed, 0);
            }
            else if(2 == cpu->job->priority) {
                removeFromCPU_nonHPF(cpu, queue, completed, 1);
            }
            else if(3 == cpu->job->priority) {
                removeFromCPU_nonHPF(cpu, queue, completed, 2);
            }
            else if(4 == cpu->job->priority) {
                removeFromCPU_nonHPF(cpu, queue, completed, 3);
            }
        } else if (!cpu->available && 1 < jobs[indexJob].remaining_service_time) {
            jobs[indexJob].remaining_service_time -= 1;
        }
        
        // check if there is a job on the CPU
        if(cpu->available) { // CPU is available
            // check if there is a job in the highest priority queue
            if(!isEmpty(&queue[0])) {
                // move the job from the queue to the CPU
                moveToCPU_nonHPF(cpu, &queue[0]);
            }
            // else check the next priority queue
            else if(!isEmpty(&queue[1])) {
                // move the job from the queue to the CPU
                moveToCPU_nonHPF(cpu, &queue[1]);
            }
            // else check the next priority queue
            else if(!isEmpty(&queue[2])) {
                // move the job from the queue to the CPU
                moveToCPU_nonHPF(cpu, &queue[2]);
            }
            // else check the next priority queue
            else if(!isEmpty(&queue[3])) {
                // move the job from the queue to the CPU
                moveToCPU_nonHPF(cpu, &queue[3]);
            }
            // otherwise the CPU remains idle
        }
        
        // increment the global clock
        clock_cpu_HPF++;
    }
    // free the memory in the CPU struct
    free(cpu->job);
}

// this function moves a job from the queue to the CPU
void moveToCPU_nonHPF(CPU* c, Queue* q) {
    c->job->pid = q->head->job.pid;
    c->job->arrival_time = q->head->job.arrival_time;
    c->job->service_time = q->head->job.service_time;
    c->job->priority = q->head->job.priority;
    c->job->remaining_service_time = q->head->job.remaining_service_time;
    // check if this is the first time the job is using the CPU
    if(1 > q->head->job.start_time) {
        c->job->start_time = clock_cpu_HPF;
    }
    else {
        c->job->start_time = q->head->job.start_time;
    }
    c->job->finish_time = q->head->job.finish_time;
    // set the age of the process and return it to zero when it gets the CPU
    c->job->age = 0;
    // remove the job from the queue
    pop(q);
    // set the cpu to being available
    c->available = false;
}

// this function removes a job from the CPU and puts it in either
// the job queue or the completed jobs array
void removeFromCPU_nonHPF(CPU* c, Queue* q, Job* complete, int queueIndex) {
    // set the job into the cpu time table
    if(256 > indexResult) {
        // set the entry for cpu usage results_Queue
        results_Queue[indexResult].pid = c->job->pid;
        results_Queue[indexResult].start = clock_cpu_HPF - 1;
        results_Queue[indexResult].end = clock_cpu_HPF;
        indexResult++;
    }
    
    // increment the ages for all waiting processes
    ageProcesses_HPF(q);
    // decrement the remaining service time for the job
    c->job->remaining_service_time -= quanta_nonHPF;
    // check if the job needs to go back into the queue or if it goes in the completed jobs list
    if(1 > c->job->remaining_service_time) {
        // job has finished so it can be replaced in the completed jobs list
        complete[indexFinished].pid = c->job->pid;
        complete[indexFinished].arrival_time = c->job->arrival_time;
        complete[indexFinished].service_time = c->job->service_time;
        complete[indexFinished].priority = c->job->priority;
        complete[indexFinished].remaining_service_time = c->job->remaining_service_time;
        complete[indexFinished].start_time = c->job->start_time;
        complete[indexFinished].finish_time = clock_cpu_HPF;
        // increment the finished index counter
        indexFinished++;
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
void removeJobFromQueue_nonHPF(Queue* q, Job* complete) {
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
            complete[indexFinished].pid = q->head->job.pid;
            complete[indexFinished].arrival_time = q->head->job.arrival_time;
            complete[indexFinished].service_time = q->head->job.service_time;
            complete[indexFinished].priority = q->head->job.priority;
            complete[indexFinished].remaining_service_time = q->head->job.remaining_service_time;
            complete[indexFinished].start_time = q->head->job.start_time;
            complete[indexFinished].finish_time = clock_cpu_HPF;
            // increment the finished index counter
            indexFinished++;
        }
        pop(q);
    }
    // reload all jobs that have already started into the job queue
    while(!isEmpty(&temp)) {
        push(q, temp.head->job);
        pop(&temp);
    }
}

// this function will increment the ages of all the waiting processes
// all queues with processes will have their age incremented with the
// exception of the highest priority queue
void ageProcesses_HPF(Queue* q) {
    // create an array to hold all waiting jobs
    int jobSize = queue_size(&q[1]) + queue_size(&q[2]) + queue_size(&q[3]);
    // if there are no waiting jobs return from the function
    if(1 > jobSize) {
        return;
    }
    // create an array to hold all of the jobs
    Job* j = malloc(sizeof(Job) * jobSize);
    int index = 0;
    // iterate through all 3 job queues
    for(int i = 1; i < 4; i++) {
        while(!isEmpty(&q[i])) {
            j[index] = front(&q[i]);
            // increment the age
            j[index].age++;
            // increment the index
            index++;
            pop(&q[i]);
        }
    }
    // place all the jobs back into their respective queue in proper order
    // while checking the age for if they need to be upgraded in their priority
    for(int i = 0; i < jobSize; i++) {
        // check the age of the job
        if(5 < j[i].age) {
            // increase priority
            j[i].priority--;
            // reset the age
            j[i].age = 0;
        }
        // put the job into its proper queue
        push(&q[(j[i].priority - 1)], j[i]);
    }
    // free the memory used to hold the jobs
    free(j);
}



