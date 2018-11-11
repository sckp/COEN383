#ifndef HELPER_H
#define HELPER_H

#include <unistd.h>

// function to clear out the buffer contents
void clear_buffer(char* msg, int size);

// function to read from a file descriptor until a
// new line character is encountered
// return the number of character read
int read_by_line(int fd, char* msg, int size);


#endif
