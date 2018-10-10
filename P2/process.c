#include <stdlib.h>
#include "process.h"

/*
 * get the random int in range (min, max)
 */
int getRandomInt(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

/*
 * generate one new procecss
 */
Process newRandomProcess()
{
    Process* p = (Process*) malloc(sizeof(Process));

    // Initialize random process
    (*p).priority = getRandomInt(1, 4);
    (*p).age = 0;
    (*p).arrival = ((float) getRandomInt(0, 990)) / 10;
    (*p).runtime = ((float) getRandomInt(1, 100)) / 10;
    (*p).timeRemaining = (*p).runtime;

    // Initialized to account for the time it spends waiting from arrival
    // to the end of the quanta during which it arrives
    // (i.e. arrives at 1.3, all of these values start at 0.7)
    (*p).responseTime = 1.0f - ((*p).arrival - ((int) (*p).arrival));
    (*p).waitTime = 1.0f - ((*p).arrival - ((int) (*p).arrival));
    (*p).turnaroundTime = 1.0f - ((*p).arrival - ((int) (*p).arrival));
    (*p).timeFinished = 0;
    return *p;
}

/*
 * new RandomProcessQueue by call queuesize times new RandomProcess();
 */
Process* randomProcessQueue(int queueSize)
{
    Process* queue = (Process*) malloc(queueSize * sizeof(Process));
    int i = 0;
    while(i < queueSize)
    {
        queue[i++] = newRandomProcess();
    }
    return queue;
}

/*
 * update the ready queue by adding upcoming process int queue at time index.
 */
void updateReadyQueue(Process* queue, Process* ready, int index, int* size, int PROCESS_COUNT)
{
    if(index == 0)
    {
        return;
    }

    int i;
    for(i = *size; i < PROCESS_COUNT; i++)
    {
        if ((queue[i].arrival > index - 1) && (queue[i].arrival <= index))
        {
            ready[*size] = queue[i];
            *size = i + 1;
        } else
        {
            return;
        }
    }
}

//------------------------------------ Sort by Arrival Time --------------------------------------
/*
 * Sort Processes By arrival time by using merge sort.
 */
void sortByArrival(Process* process, int low, int high)
{
    if(high <= low)
    {
        return;
    }
    int mid = (high + low) / 2;
    sortByArrival(process, low, mid);
    sortByArrival(process, mid + 1, high);
    int i = low;
    int j = mid + 1;
    int buff = 0;
    while(i <= mid + buff && j <= high)
    {
        if(process[j].arrival < process[i].arrival)
        {
            Process temp = process[j];
            int a = j;
            while(a > i)
            {
                process[a] = process[a - 1];
                a--;
            }
            buff++;
            j++;
            process[i] = temp;
        }
        i++;
    }
}

/*
 * Sort Processes By arrival time and generate id.
 */
void sortProcessesByArrival(Process* process, int low, int high)
{
    sortByArrival(process, low, high);
    int i;
    for(i = 0; i <= high; i++)
    {
        process[i].id = (char) (65 + i);// i.e when i = 0 -> id = 'A'
        if (i > 25) {
            process[i].id = process[i].id + 6;
        }
    }
}
