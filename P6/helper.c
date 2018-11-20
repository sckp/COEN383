#include "helper.h"

// this function sets each character to NULL
void clear_buffer(char* msg, int size) {
	for(int i = 0; i < size; i++) {
		msg[i] = '\0';
	}
}

// this function will read character by character from the
// file descriptor until a new line character is reached
int read_by_line(int fd, char* msg, int size) {
	int value_read = 0;
	// clear the buffer prior to reading
	clear_buffer(msg, size);
	
	// read no more than the size of the char array
	for(int i = 0; i < size; i++) {
		char c;
		read(fd, &c, 1);
		// if the read character is not NULL add it to the message
		if('\n' != c) {
			msg[i] = c;
			value_read++;
		}
		else {
			msg[i] = c;
			value_read++;
			break;
		}
	}
	return value_read;
}

// this function writes out the data to the desired file
void write_to_file(int fd, char* msg, int size) {
	for(int i = 0; i < size; i++) {
		if('\0' == msg[i]) {
			break;
		}
		else {
			write(fd, &msg[i], 1);
		}
	}
}

// this function handles the actions required by the children
// that do not interact with the terminal
void no_terminal_child(int fd, int child, int buff_size, int baseTime) {
	// close terminal file descriptors
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	
	// initialize the seed for randomization
	srand(time(NULL));
	// set the end time of the while loop
	int endtime = baseTime + 30;
	// create a variable to count the number of messages
	int msg_num = 1;
	// create a write message buffer
	char* write_msg = malloc(sizeof(char) * buff_size);

	// create the struct to use gettimeofday function
	struct timeval tv;
	// get the starting time
	gettimeofday(&tv, NULL);

	// loop until the timelimit is reached
	while(endtime > (int)tv.tv_sec) {
		// sleep for the random amount of time
		sleep(rand() % 3);
		// get the time of day for the message
		gettimeofday(&tv, NULL);
		// find out the time between messages sent
		int msg_time = (int)(tv.tv_sec - baseTime);
		double msec = (double)(tv.tv_usec/1000.00);
		
		// format the message that the child will send to the parent
		snprintf(write_msg, buff_size, "%i:%05.3f: Child %i message %i\n", msg_time, msec, child, msg_num);
		// write the message to the pipe
		write(fd, write_msg, strlen(write_msg));
		
		// get the time of day for next iteration
		gettimeofday(&tv, NULL);
		msg_num++;
	}
	// write to the parent that the pipe is closing
	char end_msg[] = "END";
	write(fd, end_msg, sizeof(end_msg));
	// free the memory used for a buffer
	free(write_msg);
	
	// close the write end of the pipe before terminating the child
	close(fd);
}

// this function handles interaction with the terminal
void terminal_child(int fd, int child, int buff_size, int baseTime) {
	// create the struct to use gettimeofday function
	struct timeval tv;
	
	// initialize the seed for randomization
	srand(time(NULL));
	// create a variable to count the number of messages
	int msg_num = 1;
	
	// create a write message buffer
	char* write_msg = malloc(sizeof(char) * (2 * buff_size));
	// create flag to determine if the prompt is needed
	bool prompt = true;
	
	// read from all of the pipes and write to a file
	fd_set fdsets;
	struct timeval timeout;
	// set the timeout value
	timeout.tv_sec = 30;
	timeout.tv_usec = 0;
	bool time_limit = false;
	
	// loop for 30 seconds
	while(!time_limit) {
		// ensure that fds is zeroed
		FD_ZERO(&fdsets);
		FD_SET(STDIN_FILENO, &fdsets);
		if(prompt) {
			// set need for the prompt to false
			prompt = false;
			// display prompt to user
			char prom[] = "Enter user input (64 character limit): ";
			write(STDOUT_FILENO, prom, strlen(prom));
		}
		// use select to monitor for input
		int retval = select(12, &fdsets, NULL, NULL, &timeout);
		if(-1 == retval) {
			perror("select()");
		}
		else if(retval) {
			// make sure that the input is from STDIN_FILENO
			if(FD_ISSET(STDIN_FILENO, &fdsets)) {
				// reset the prompt flag to generate the prompt again
				prompt = true;
				// create a user input buffer
				char* user_input = malloc(sizeof(char) * buff_size);
				clear_buffer(user_input, buff_size);
				// limit the width read in to 64 characters
				read(STDOUT_FILENO, user_input, buff_size);
				
				// get the time of day for the message
				gettimeofday(&tv, NULL);
				// find out the time between messages sent
				int msg_time = (int)(tv.tv_sec - baseTime);
				double msec = (double)(tv.tv_usec/1000.00);
				
				// format the message that the child will send to the parent
				snprintf(write_msg, (2 * buff_size), "%i:%05.3f: Child %i: %s", msg_time, msec, child, user_input);
				// write the message to the pipe
				write(fd, write_msg, strlen(write_msg));
				
				// get the time of day for next iteration
				gettimeofday(&tv, NULL);
				msg_num++;
				
				// free the buffer memory
				free(user_input);
			}
		}
		else {
			printf("\n30 second limit reached\n");
			time_limit = true;
		}
		// get the time to see if child's time has elapsed
		gettimeofday(&tv, NULL);
	}
	
	// write to the parent that the pipe is closing
	char end_msg[] = "END";
	write(fd, end_msg, sizeof(end_msg));
	// free the memory used for a buffer
	free(write_msg);
	close(fd);
}

// function to write the parent timestamp to the output message
void parent_timestamp(int fd, int baseTime) {
	// create the struct to use gettimeofday function
	struct timeval tv;
	// get the time of day for the message
	gettimeofday(&tv, NULL);
	// find out the time between messages sent
	int msg_time = (int)(tv.tv_sec - baseTime);
	double msec = (double)(tv.tv_usec/1000.00);
	
	// create a write message buffer
	char* write_msg = malloc(sizeof(char) * 32);
	// format the message that the child will send to the parent
	snprintf(write_msg, 32, "%i:%05.3f: Parent: ", msg_time, msec);
	// write the message to the output file
	write_to_file(fd, write_msg, strlen(write_msg));
}
