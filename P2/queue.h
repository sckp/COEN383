#ifndef QUEUE
#define QUEUE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "jobhelper.h"

// create a node for use in the queue which
// holds the pointer to the next one and the
// job struct
struct Node {
	Job job;
	struct Node *next;
};

// create the queue struct which will hold all of
// the nodes with the job data
struct Queue {
	struct Node *first;
	struct Node *last;
	int size;
};

// create an initializer for the queue
void initialize(struct Queue *q) {
	q->first = NULL;
	q->last = NULL;
	q->size = 0;
}

// check if the queue is empty
bool isEmpty(struct Queue *q) {
	return (1 > q->size);
}

// get the queue size
int queue_size(struct Queue *q) {
	return q->size;
}

// create a function to get the front element from the queue
struct Job front(struct Queue *q) {
	return q->first->job;
}

// create a function that pops an element from the queue,
// adjusts the pointers within the queue, decrements the size,
// and frees the memory
void pop(struct Queue *q) {
	// decrement the size
	q->size--;
	//move the second node to the first position
	struct Node *temp = q->first;
	q->first = q->first->next;	
	// free the memory
	free(temp);
}

// create a function that adds an element to the queue
void push(struct Queue *q, Job job) {
	// increment the size
	q->size++;
	
	// check if this is the first element being added to the queue
	if(NULL == q->first) {
		// place the new node as the first element
		q->first = (struct Node*)malloc(sizeof(struct Node));
		// set the job
		q->first->job = job;
		// set the next element to null
		q->first->next = NULL;
		// set the last element pointing to itself
		q->last = q->first;
	}
	// else it isn't the first element
	else {
		// place the new node at the end of the queue
		q->last->next = (struct Node*)malloc(sizeof(struct Node));
		// set the job
		q->last->next->job = job;
		// set the end pointer to null
		q->last->next->next = NULL;
		// set the last pointer
		q->last = q->last->next;
	}
}

// print each element in the queue
void print_queue(struct Queue *q) {
	if(isEmpty(q)) {
		printf("Queue is empty.\n");
	}
	else {
		struct Node *n = q->first;
		// loop through all nodes until NULL is reached
		while(NULL != n) {
			print_job(n->job);
			n = n->next;
		}
	}
}

// empty the queue
void make_empty(struct Queue *q) {
	// iterate through the queue freeing memory
	while(NULL != q->first) {
		struct Node *temp = q->first;
		q->first = q->first->next;
		free(temp);
	}
}


#endif
