#ifndef HELPER_H
#define HELPER_H

#include <stdio.h> // for terminal
#include <string.h> // strcmp

#include <time.h>
#include <stdlib.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

// function to clear out the buffer contents
void clear_buffer(char* msg, int size);

// function to read from a file descriptor until a
// new line character is encountered
// return the number of character read
int read_by_line(int fd, char* msg, int size);

// function to write the data to the file
void write_to_file(int fd, char* msg, int size);

// function to handle non terminal children
void no_terminal_child(int fd, int child, int buff_size, int baseTime);

// function to handle child that interacts with terminal
void terminal_child(int fd, int child, int buff_size, int baseTime);

// function to write parent timestamp message
void parent_timestamp(int fd, int baseTime);

#endif
