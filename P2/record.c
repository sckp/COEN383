#include <stdlib.h>
#include <stdio.h>
#include "process.c"
#include "record.h"

Record newRecord()
{
    Record record = *((Record*) malloc(sizeof(Record)));
    record.processCount = 0;
    record.timesliceCount = 0;
    return record;
}

/*
 * Add Process into Rocord.process array
 */
void addProcess(Record* r, Process p)
{
    (*r).processCount = (*r).processCount + 1;
    Process* newArray = (Process*) malloc(sizeof(Process) * (*r).processCount);
    int i;
    for(i = 0; i < (*r).processCount - 1; i++)
    {
        newArray[i] = (*r).process[i];
    }
    newArray[(*r).processCount - 1] = p;
    (*r).process = newArray;
}

/*
 * add Timeslice into Rocord.timeslice array
 */
void addTimeslice(Record* r, Timeslice t)
{
    (*r).timesliceCount = (*r).timesliceCount + 1;
    Timeslice* newArray = (Timeslice*) malloc(sizeof(Timeslice) * (*r).timesliceCount);
    int i;
    for(i = 0; i < (*r).timesliceCount - 1; i++)
    {
        newArray[i] = (*r).timeslice[i];
    }
    newArray[(*r).timesliceCount - 1] = t;
    (*r).timeslice = newArray;
}

/*
 * Print the result
 */
void printRecord(Record r)
{
    // print simulation process
    printf("|-----+---------+----------+----------+----------+------+------------+--------|\n");
    printf("| PID | Arrival | Priority | Duration | Response | Wait | Turnaround | Finish |\n");
    printf("|-----+---------+----------+----------+----------+------+------------+--------|\n");
    int i;
    float totalResponse = 0.0f;
    float totalWait = 0.0f;
    float totalTurnaround = 0.0f;
    for(i = 0; i < r.processCount; i++)
    {
        printf("| %3c | %7.1f | %8d | %8.1f | %8.1f | %4.1f | %10.1f | %6d |\n", r.process[i].id, r.process[i].arrival, r.process[i].priority, r.process[i].runtime, r.process[i].responseTime, r.process[i].waitTime, r.process[i].turnaroundTime, r.process[i].timeFinished);
        totalResponse += r.process[i].responseTime;
        totalWait += r.process[i].waitTime;
        totalTurnaround += r.process[i].turnaroundTime;
    }
    printf("|-----+---------+----------+----------+----------+------+------------+--------|\n");

    // print the timeslice and corresponding process
    printf("\n");
    printf("|-----------+---------|\n");
    printf("| Timeslice | Process |\n");
    printf("|-----------+---------|\n");
    for(i = 0; i < r.timesliceCount; i++)
    {
        printf("| %9d | %7c |\n", r.timeslice[i].index, r.timeslice[i].pid);
    }
    printf("|-----------+---------|\n");

    // print the calculated result
    printf("\n");
    printf(" --- Calculated Averages Result --- \n");
    printf("  Response: %.1f\n", totalResponse / r.processCount);
    printf("      Wait: %.1f\n", totalWait / r.processCount);
    printf("Turnaround: %.1f\n", totalTurnaround / r.processCount);
    printf("\n");
    printf(" --- Calculated Throughput --- \n");
    printf(" Processes: %d\n", r.processCount);
    printf("      Time: %d\n", r.timesliceCount);
    printf("Throughput: %.1f processes per 100 quanta\n", 100 * ((float) r.processCount) / r.timesliceCount);
}

