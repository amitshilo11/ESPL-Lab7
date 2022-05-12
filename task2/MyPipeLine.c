#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#include "LineParser.h"

#define WRITE_END   1
#define READ_END    0
#define CHICO       0

#define ERR_MSG(S, M)   if(S<0)  {fprintf(stderr, "ERROR: %s EXITTING\n", M); exit(1);}
#define DBG_MSG(M)      if(d_flag) fprintf(stderr, "DEBUGER: %s\n", M);
#define DBG_MSGN(M, N)  if(d_flag) fprintf(stderr, "DEBUGER: %s %d\n", M, N);

extern int execute(cmdLine *pCmdLine);

void my_pipe(cmdLine* first_line, cmdLine* second_line) {
    int fd[2], pid;
    int d_flag = 1;

    int pipe_status = pipe(fd);
    ERR_MSG(pipe_status, "pipe failed");

    DBG_MSG("forking...")
    pid = fork();
    DBG_MSGN("created process with id: ", pid)
    ERR_MSG(pid, "fork on pid1 failed")

    if(pid == CHICO) {
        DBG_MSG("child1 > redirecting stdout to the write end of the pipe...")
        close(STDOUT_FILENO);
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[WRITE_END]);
        DBG_MSG("child1 > going to execute cmd: ...")
        execute(first_line);
        //execvp(first_line->arguments[0], first_line->arguments);
        //fprintf(stderr, "Failed to execute\n");
        exit(1);
    }
    else {
        DBG_MSG("parent_process > closing the write end of the pipe...")
        close(fd[WRITE_END]);
        DBG_MSG("parent_process > forking...")
        pid = fork();
        DBG_MSGN("parent_process > created process with id: ", pid)

        ERR_MSG(pid, "fork on pid2 failed")

        if(pid == CHICO) {
            DBG_MSG("child2 > redirecting stdin to the read end of the pipe...")
            close(STDIN_FILENO);
            dup2(fd[READ_END], STDIN_FILENO);
            close(fd[READ_END]);
            DBG_MSG("child2 > going to execute cmd: ...")
            execute(second_line);
            //execvp(second_line->arguments[0], second_line->arguments);
            //fprintf(stderr, "Failed to execute\n");
            exit(1);
        }
        else {
            DBG_MSG("parent_process > closing the read end of the pipe...")
            close(fd[READ_END]);
            int status;
            DBG_MSG("parent_process > waiting for child processes to terminate...")
            waitpid(pid, &status, 0);
            DBG_MSG("parent_process > exiting...")
        }
    }
}