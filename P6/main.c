#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#define BUFF_SIZE		32
#define READ_END		0
#define WRITE_END		1
#define CHILDREN		5

int main(int argc, char* argv[]) {
	// create a write and read message buffer
	char write_msg[BUFF_SIZE];
	
	//char read_msg[BUFF_SIZE];
	
	// create array to hold the five child processes ID
	pid_t cpid[CHILDREN];
	
	// file descriptors for the pipe for the 5 children
	int fd[CHILDREN][2];
	
	// create a set of pipes for each child
	for(int i = 0; i < CHILDREN; i++) {
		// create the pipe
		if(pipe(fd[i]) == -1) {
			fprintf(stderr, "pipe() failed");
			return 1;
		}
	}
	
	// use a for loop to create each of the children
	for(int i = 0; i < CHILDREN; i++) {
		cpid[i] = fork();
		// check if it is the parent
		if(0 < cpid[i]) {
			// close the write end of the parent to child pipe
			close(fd[i][WRITE_END]);
			
			
		}
		// check if it is a child
		else if(0 == cpid[i]) {
			// close the read end of the childs pipe
			close(fd[i][READ_END]);
						
			// initialize the seed for randomization
			int seed = time(NULL);
			srand(seed);
			
			// create the struct to use gettimeofday function
			struct timeval tv;
			// get the starting time
			gettimeofday(&tv, NULL);
			// set the end time of the while loop
			int endtime = tv.tv_sec + 10;
			// set the baseline time that needs to be subtracted
			int baseTime = tv.tv_sec;
			// create a variable to count the number of messages
			int msg_num = 1;
			
			// check if it is child 1-4 which does not use the terminal
			if(4 > i) {
				//close(STDOUT_FILENO);
				//close(STDIN_FILENO);
				while(endtime > (int)tv.tv_sec) {
					// get a random sleep time
					int sleep_time = rand() % 3;
					// sleep for the random amount of time
					sleep(sleep_time);
					// get the time of day for the message
					gettimeofday(&tv, NULL);
					
					// find out the time between messages sent
					int msg_time = (int)(tv.tv_sec - baseTime);
					// get the msec
					double msec = (double)(tv.tv_usec/1000);
					
					/*
					 * NEED TO ADJUST THE TIME STAMP FORMATTING
					 * 
					*/
					
					// write the child's message to the pipe
					sprintf(write_msg, "%i:%05.3f: Child %i message %i\n", msg_time, msec, i, msg_num);
					
					// write the message to the pipe
					write(fd[i][WRITE_END], write_msg, strlen(write_msg));
					
					// get the time of day for next iteration
					gettimeofday(&tv, NULL);
					msg_num++;
					
				}
				
			}
			// otherwise it is the child that reads from the terminal
			else {
				while(endtime > (int)tv.tv_sec) {
					char user_input[64];
					// prompt for user input
					printf("Please input a string no longer than 64 characters: ");
					scanf("%s", user_input);
					
					// get the time of day for the message
					gettimeofday(&tv, NULL);
					
					// find out the time between messages sent
					int msg_time = (int)(tv.tv_sec - baseTime);
					// get the msec
					double msec = (double)(tv.tv_usec/1000);
					
					// create a buffer to use for data written to the pipe
					char buff[100];
					
					/*
					 * NEED TO ADJUST THE TIME STAMP FORMATTING
					 * AND THE CONSOLE MESSAGE CONCATENATION
					*/
					
					// write the child's message to the pipe
					sprintf(buff, "%i:%05.3f: Child %i: %i %s\n", msg_time, msec, i, msg_num, buff);
					
					// write the message to the pipe
					write(fd[i][WRITE_END], buff, strlen(buff));
					
					// get the time of day for next iteration
					gettimeofday(&tv, NULL);
					msg_num++;
				}
			}
			
			// close the write end of the pipe before terminating the child
			close(fd[i][WRITE_END]);
			// have the child process exit
			_exit(0);
		}
		// else fork failed
		else {
			fprintf(stderr, "fork() failed");
			return 1;
		}
		
		// read from all of the pipes and write to a file
		/*
		 * 
		 * READ FROM ALL OF THE PIPES USING SELECT
		 * ONCE ALL PIPES HAVE BEEN CLOSED CLOSE THE FILE
		 * AND TERMINATE
		 * 
		*/
		
	}
	
	
	return 0;
}
