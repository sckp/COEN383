#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#include "helper.h"

#define BUFF_SIZE		64
#define READ_END		0
#define WRITE_END		1
#define KIDS			4

int main(int argc, char* argv[]) {
	// variable for child process
	pid_t cpid[KIDS];
	// file descriptors for the pipe for the child
	int fd[KIDS][2];
		
	// create a set of pipes for each child
	for(int i = 0; i < KIDS; i++) {
		if(pipe(fd[i]) == -1) {
			fprintf(stderr, "pipe() failed");
			return 1;
		}
	}
	
	// loop to create all of the desired children
	for(int i = 0; i < KIDS; i++) {
		// fork a child process
		cpid[i] = fork();
		// check if it is the parent
		if(0 < cpid[i]) {
			// close the write end of the parent to child pipe
			close(fd[i][WRITE_END]);
		}
		// check if it is a child
		else if(0 == cpid[i]) {
			// close unused child file descriptors
			close(fd[i][READ_END]);
			// check if it is one of the non terminal children
			if(4 > i) {
				no_terminal_child(fd[i][WRITE_END], i, BUFF_SIZE);
			}
			// otherwise it is the terminal child
			else {
				
			}
			
			// have the child process exit
			_exit(EXIT_SUCCESS);
		}
		// else fork failed
		else {
			fprintf(stderr, "fork() failed");
			return 1;
		}
	}
	
	// set the mode for the file to be opened/created
	mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
	int outFD = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, mode);
	if(0 > outFD) {
		fprintf(stderr, "open() failed");
		exit(EXIT_FAILURE);
	}
	
	// read from all of the pipes and write to a file
	fd_set fdsets;
	struct timeval timeout;
	
	// create bool variable to signify a pipe being open
	bool p1 = true, p2 = true, p3 = true, p4 = true;
	// loop as long as a pipe is open
	while(p1 || p2 || p3 || p4) {
		// set the timeout value
		timeout.tv_sec = 2;
		timeout.tv_usec = 500000;
		// ensure that fds is zeroed
		FD_ZERO(&fdsets);
		// set all of the file descriptors
		for(int i = 0; i < KIDS; i++) {
			FD_SET(fd[i][READ_END], &fdsets);
		}
		
		int retval = select(12, &fdsets, NULL, NULL, &timeout);
		if(-1 == retval) {
			perror("select()");
		}
		else if(retval) {
			// create a buffer for the message to be read in to
			char* read_msg = malloc(sizeof(char) * BUFF_SIZE);
			// read from the selected file descriptor and write it to the file
			// first child's pipe
			if(FD_ISSET(fd[0][READ_END], &fdsets)) {
				int val = read_by_line(fd[0][READ_END], read_msg, BUFF_SIZE);
				// check to see if the pipe has been closed by the child
				if(0 == strcmp("END", read_msg)) {
					// set the pipe open flag to false and close the pipe
					p1 = false;
				}
				// otherwise write the data to the file
				else {
					write_to_file(outFD, read_msg, val);
				}
			}
			// second child's pipe
			if(FD_ISSET(fd[1][READ_END], &fdsets)) {
				int val = read_by_line(fd[1][READ_END], read_msg, BUFF_SIZE);
				// check to see if the pipe has been closed by the child
				if(0 == strcmp("END", read_msg)) {
					// set the pipe open flag to false and close the pipe
					p2 = false;
				}
				// otherwise write the data to the file
				else {
					write_to_file(outFD, read_msg, val);
				}
			}
			// third child's pipe
			if(FD_ISSET(fd[2][READ_END], &fdsets)) {
				int val = read_by_line(fd[2][READ_END], read_msg, BUFF_SIZE);
				// check to see if the pipe has been closed by the child
				if(0 == strcmp("END", read_msg)) {
					// set the pipe open flag to false and close the pipe
					p3 = false;
				}
				// otherwise write the data to the file
				else {
					write_to_file(outFD, read_msg, val);
				}
			}
			// fourth child's pipe
			if(FD_ISSET(fd[3][READ_END], &fdsets)) {
				int val = read_by_line(fd[3][READ_END], read_msg, BUFF_SIZE);
				// check to see if the pipe has been closed by the child
				if(0 == strcmp("END", read_msg)) {
					// set the pipe open flag to false and close the pipe
					p4 = false;
				}
				// otherwise write the data to the file
				else {
					write_to_file(outFD, read_msg, val);
				}
			}
			// free the buffer memory
			free(read_msg);
		}
	}
	// close all of the pipes to the children
	for(int i = 0; i < KIDS; i++) {
		close(fd[i][READ_END]);
	}
	
	
	// close the output file descriptor
	close(outFD);
	
	printf("Program done\n");
	
	return 0;
}
