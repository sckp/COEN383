#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFF_SIZE	32
#define R_END		0
#define W_END		1

int main(int argc, char* argv[]) {
	char w_msg[] = "You're my child process.";
	char r_msg[BUFF_SIZE];
	
	pid_t pid; // child process id
	
	// file descriptors for the pipe
	int fd[2];
	
	// create the pipe
	if(pipe(fd) == -1) {
		fprintf(stderr, "pipe() failed");
		return 1;
	}
	
	// fork a child process
	pid = fork();
	if(0 < pid) {
		// parent process
		
		// close the unused read end of the pipe
		close(fd[R_END]);
		
		// write to the end of the pipe
		write(fd[W_END], w_msg, strlen(w_msg) + 1);
		printf("Parent: Wrote '%s' to the pipe.\n", w_msg);
		
		// close the write end of the pipe
	}
	else if(0 == pid) {
		// child process
		
		// close the unused write end
		close(fd[W_END]);
		
		// read from the read end of the pipe
		read(fd[R_END], r_msg, BUFF_SIZE);
		printf("Child: Read '%s' from pipe.\n", r_msg);
		
		// close the read end of the pipe
		close(fd[R_END]);
	}
	else {
		fprintf(stderr, "fork() failed");
		return 1;
	}
	
	return 0;
}
