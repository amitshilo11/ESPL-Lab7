#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int fd[2], pid, size;
    char buf[20];
    char* message = "Hello";

    if(pipe(fd) == -1) {
        fprintf(stderr, "%s\n", "pipe FAILED");
        exit(1);
    }

    pid = fork();
    if(pid < 0) 
        fprintf(stderr, "%s\n", "fork FAILED");
    
    if(pid > 0) {     // papa
        write(fd[1], message, strlen(message)+1);
        close(fd[1]);
        _exit(0);

    }
    else {    // chico
        wait(NULL);
        while((size = read(fd[0], buf, 6) > 0)) {
            fprintf(stdout, "%s\n", buf);
        }
        if(size == -1) {
            fprintf(stderr, "%s\n", "read FAILED");
            exit(2);
        }
    }
    
	return 0;
}