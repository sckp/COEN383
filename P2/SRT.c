#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "record.c"

#define PROCESS_COUNT 52
#define SIMULATION_LENGTH 100

/*
 * find the index of imcomplete process in ready queue which has the shortest runtime
 */
void findShortestProcessIndex(Process* ready, int* spIndex, int size)
{
    int i = 0;
    float currentShortestRuntime = 11.0f;
    for(i; i < size; i++)
    {
        if((ready[i].timeRemaining < currentShortestRuntime) && (ready[i].timeRemaining > 0.0f))
        {
            currentShortestRuntime = ready[i].timeRemaining;
            *spIndex = i;
        }
    }
}

/*
 * Shortest remaining time
 */
int main()
{
    int i = 0;
    for(; i < 5; i++){
        srand(i); // use seed "time(0)" to initialize rand function
        Process* arrivalQueue = randomProcessQueue(PROCESS_COUNT); // generate rand pricess Queue
        sortProcessesByArrival(arrivalQueue, 0, PROCESS_COUNT - 1); // sort the queue by arrival time
        Process* readyQueue = (Process*) malloc(PROCESS_COUNT * sizeof(Process)); // store the arrived process
        
        int i = 0; // cur time slice index
        int size = 0; // ready queue size
        int shortestProcessIndex = 0;
        bool finished = false;
        Record record = newRecord();
        while(!finished)
        {
            Timeslice* timeslice = (Timeslice*) malloc(sizeof(Timeslice));
            (*timeslice).index = i;
            (*timeslice).pid = '-';
            updateReadyQueue(arrivalQueue, readyQueue, i, &size, PROCESS_COUNT);
            findShortestProcessIndex(readyQueue, &shortestProcessIndex, size);
            
            // when ready queue is not empty and shortest Process is still running.
            // we can continue dealing with current process and record it.
            if(size > 0 && (readyQueue[shortestProcessIndex].timeRemaining > 0))
            {
                (*timeslice).pid = readyQueue[shortestProcessIndex].id;
                
                // when start this process
                if(readyQueue[shortestProcessIndex].runtime == readyQueue[shortestProcessIndex].timeRemaining)
                {
                    readyQueue[shortestProcessIndex].responseTime = ((float) i) - readyQueue[shortestProcessIndex].arrival;
                }
                
                // update update the status(waitTime and timeRemaining) of the process which has already arrived
                readyQueue[shortestProcessIndex].timeRemaining = readyQueue[shortestProcessIndex].timeRemaining - 1.0f;
                int j;
                for(j = 0; j < size; j++)
                {
                    if(j != shortestProcessIndex) readyQueue[j].waitTime += 1.0f;
                }
                
                // when we finished current process, update the current process status and record it
                if(readyQueue[shortestProcessIndex].timeRemaining <= 0)
                {
                    readyQueue[shortestProcessIndex].timeFinished = i + 1;
                    readyQueue[shortestProcessIndex].turnaroundTime = ((float) i + 1) - readyQueue[shortestProcessIndex].arrival + readyQueue[shortestProcessIndex].timeRemaining;
                    addProcess(&record, readyQueue[shortestProcessIndex]);
                    if(i >= SIMULATION_LENGTH)
                    {
                        finished = true;
                    }
                }
            }
            
            addTimeslice(&record, (*timeslice));
            i++;
        }
        
        // print the result.
        printRecord(record);
    }
    
    return 0;
}


