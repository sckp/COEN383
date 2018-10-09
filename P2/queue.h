#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "jobhelper.h"

// create a node for use in the queue which
// holds the pointer to the next one and the
// job struct
struct Node {
	Job job;
	struct Node* next;
} Node;

// create the queue struct which will hold all of
// the nodes with the job data
typedef struct Queue {
	struct Node* head;
	struct Node* tail;
	int size;
} Queue;

// create an initializer for the queue
void initialize(struct Queue* q);

// check if the queue is empty
bool isEmpty(struct Queue* q);

// get the queue size
int queue_size(struct Queue* q);

// create a function to get the front element from the queue
struct Job front(struct Queue* q);

// create a function that pops an element from the queue,
// adjusts the pointers within the queue, decrements the size,
// and frees the memory
void pop(struct Queue* q);

// create a function that adds an element to the queue
void push(struct Queue* q, Job job);

// print each element in the queue
void print_queue(struct Queue* q);

// empty the queue
void make_empty(struct Queue* q);

// sort queue based on priority
void prioritySort(struct Queue* q);

// sort queue based on service time
void serviceSort(struct Queue* q);

// sort queue based on service time
void remainingServiceSort(struct Queue* q);





#endif
