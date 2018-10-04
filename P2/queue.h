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
	struct Node *head;
	struct Node *tail;
	int size;
};

// create an initializer for the queue
void initialize(struct Queue *q) {
	q->head = NULL;
	q->tail = NULL;
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
	return q->head->job;
}

// create a function that pops an element from the queue,
// adjusts the pointers within the queue, decrements the size,
// and frees the memory
void pop(struct Queue *q) {
	// decrement the size
	q->size--;
	//move the second node to the first position
	struct Node *temp = q->head;
	q->head = q->head->next;	
	// free the memory
	free(temp);
}

// create a function that adds an element to the queue
void push(struct Queue *q, Job job) {
	// increment the size
	q->size++;
	// check if this is the first element being added to the queue
	if(NULL == q->head) {
		// place the new node as the first element
		q->head = (struct Node*)malloc(sizeof(struct Node));
		// set the job
		q->head->job = job;
		// set the next element to null
		q->head->next = NULL;
		// set the last element pointing to itself
		q->tail = q->head;
	}
	// else it isn't the first element
	else {
		// place the new node at the end of the queue
		q->tail->next = (struct Node*)malloc(sizeof(struct Node));
		// set the job
		q->tail->next->job = job;
		// set the end pointer to null
		q->tail->next->next = NULL;
		// set the last pointer
		q->tail = q->tail->next;
	}
}

// print each element in the queue
void print_queue(struct Queue *q) {
	if(isEmpty(q)) {
		printf("Queue is empty.\n");
	}
	else {
		struct Node *n = q->head;
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
	while(NULL != q->head) {
		struct Node *temp = q->head;
		q->head = q->head->next;
		free(temp);
	}
}

// sort queue based on priority
void prioritySort(struct Queue *q) {
	// check if the queue is empty
	if(isEmpty(q)) {
		return;
	}
	// queue is not empty sort based on highest priority
	else {
		// find the size of the queue
		int qSize = queue_size(q);
		// create an array of Jobs
		struct Job j[qSize];
		for(int i = 0; i < qSize; i++) {
			// set the job in the array
			j[i].pid = q->head->job.pid;
			j[i].arrival_time = q->head->job.arrival_time;
			j[i].service_time = q->head->job.service_time;
			j[i].priority = q->head->job.priority;
			j[i].remaining_service_time = q->head->job.remaining_service_time;
			j[i].start_time = q->head->job.start_time;
			j[i].finish_time = q->head->job.finish_time;
			// remove the element from the queue
			pop(q);
		}
		// sort the array based on priority
		job_sort(j, qSize, 2);
		// recreate the queue based on sorted data
		for(int i = 0; i < qSize; i++) {
			push(q, j[i]);
		}
	}
}

// sort queue based on service time
void serviceSort(struct Queue *q) {
	// check if the queue is empty
	if(isEmpty(q)) {
		return;
	}
	// queue is not empty sort based on highest priority
	else {
		// find the size of the queue
		int qSize = queue_size(q);
		// create an array of Jobs
		struct Job j[qSize];
		for(int i = 0; i < qSize; i++) {
			// set the job in the array
			j[i].pid = q->head->job.pid;
			j[i].arrival_time = q->head->job.arrival_time;
			j[i].service_time = q->head->job.service_time;
			j[i].priority = q->head->job.priority;
			j[i].remaining_service_time = q->head->job.remaining_service_time;
			j[i].start_time = q->head->job.start_time;
			j[i].finish_time = q->head->job.finish_time;
			// remove the element from the queue
			pop(q);
		}
		// sort the array based on priority
		job_sort(j, qSize, 1);
		// recreate the queue based on sorted data
		for(int i = 0; i < qSize; i++) {
			push(q, j[i]);
		}
	}
}

// sort queue based on service time
void remainingServiceSort(struct Queue *q) {
	// check if the queue is empty
	if(isEmpty(q)) {
		return;
	}
	// queue is not empty sort based on highest priority
	else {
		// find the size of the queue
		int qSize = queue_size(q);
		// create an array of Jobs
		struct Job j[qSize];
		for(int i = 0; i < qSize; i++) {
			// set the job in the array
			j[i].pid = q->head->job.pid;
			j[i].arrival_time = q->head->job.arrival_time;
			j[i].service_time = q->head->job.service_time;
			j[i].priority = q->head->job.priority;
			j[i].remaining_service_time = q->head->job.remaining_service_time;
			j[i].start_time = q->head->job.start_time;
			j[i].finish_time = q->head->job.finish_time;
			// remove the element from the queue
			pop(q);
		}
		// sort the array based on priority
		job_sort(j, qSize, 3);
		// recreate the queue based on sorted data
		for(int i = 0; i < qSize; i++) {
			push(q, j[i]);
		}
	}
}





#endif
