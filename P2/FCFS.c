#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "FCFS.h"

void jobs_FCFS(struct Job* jobs, struct Job* finished, int numJobs) {
    // clock variables
    int cpu_clock = 0;
    int max_time = 100;
    
    // whether there is an element that should be worked on loaded in the queue
    bool cpu_occupied  = false;

    struct Job* queue = malloc(sizeof(Job) * numJobs);

    // pointers to increment through jobs and finished
    Job* jobs_increment = jobs;
    Job* finished_increment = finished;

    // keeping track of where in the queue I am and where I should add next element
    int job_queue_front = 0;
    int job_queue_back = -1;

    // keeping track of jobs that have entered the queue and jobs that have finished
    int jobs_queued = 0;
    int jobs_finished = 0;

    // sort jobs by arrival time
    job_sort(jobs, numJobs, 0);

    // start processing loop
    while(cpu_clock < max_time) {

        // load any new jobs into the queue
        while(jobs_queued < numJobs && jobs_increment->arrival_time == cpu_clock) {
            jobs_increment->start_time = cpu_clock;
            job_queue_back++;
            queue[job_queue_back] = *jobs_increment;
            jobs_increment++;
            jobs_queued++;
        }

        // if there is nothing currently in the queue, than the cpu is not occupied and there is no need to work on a job
       if(job_queue_front > job_queue_back) {
           cpu_occupied = false;
       } else {
           // queue has an element so there is work being done
           cpu_occupied = true;
       }

        // cpu is occupied, so a job should be worked on
        if(cpu_occupied) {
            queue[job_queue_front].remaining_service_time--;
            if(queue[job_queue_front].remaining_service_time == 0) {
                *finished_increment = queue[job_queue_front];
                finished_increment->finish_time = cpu_clock;
                finished_increment++;
                jobs_finished++;
                job_queue_front++;
            }
          }
        // incrememnt clock
        cpu_clock++;
    }

    printf("\nDone with loading. Jobs finished = %d\n job queue back = %d\n\n", jobs_finished, job_queue_back);


    // I have commented out this block because for this scheduling algorithm, I don't think I want to do this

    // finsih up everything else in the queue that was added before the clock went to 100
    // while(job_queue_front <= job_queue_back) {
    //   queue[job_queue_front].remaining_service_time--;
    //   if(queue[job_queue_front].remaining_service_time == 0) {
    //         *finished_increment = queue[job_queue_front];
    //         finished_increment->finish_time = cpu_clock;
    //         print_job(*finished_increment);
    //         finished_increment++;
    //         jobs_finished++;
    //         job_queue_front++;
    //     }
    //     cpu_clock++;
    // }

    // free memory from the temporary queue I made
    free(queue);
}
