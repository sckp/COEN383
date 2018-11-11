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
