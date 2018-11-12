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
void no_terminal_child(int fd, int child, int buff_size) {
	// close terminal file descriptors
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	// create the struct to use gettimeofday function
	struct timeval tv;
	
	// initialize the seed for randomization
	srand(time(NULL));
	// get the starting time
	gettimeofday(&tv, NULL);
	// set the end time of the while loop
	int endtime = tv.tv_sec + 7;
	// set the baseline time that needs to be subtracted
	int baseTime = tv.tv_sec;
	// create a variable to count the number of messages
	int msg_num = 1;
	
	// create a write message buffer
	char* write_msg = malloc(sizeof(char) * buff_size);
	// check if it is the first four children
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
void terminal_child(int fd) {
	// write to the parent that the pipe is closing
	char end_msg[] = "END";
	write(fd, end_msg, sizeof(end_msg));
	
	// close the write end of the pipe before terminating the child
	close(fd);
}
