#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "LineParser.h"

int BUF_SIZE = 2048;
char* PIPE_TOKEN = "|";

int cmp_arg0(cmdLine* cmd_line, char* str) {
    if(strcmp(cmd_line->arguments[0], str) == 0)
        return 1;
    return 0;
}


int execute(cmdLine *pCmdLine) {
    //char err_str[1024];
    int status, pid;

    if(!(pid = fork())) {       // chico
        if (pCmdLine->inputRedirect != NULL) {
            fclose(stdin);
            fopen(pCmdLine->inputRedirect, "r");
        }

        if (pCmdLine->outputRedirect != NULL) {
            fclose(stdout);
            fopen(pCmdLine->outputRedirect, "a");
        }
        execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        perror("");
        exit(1);
    }
    
    if(pCmdLine->blocking == 1)
        waitpid(pid, &status, 0);
}

extern void my_pipe(cmdLine* first_line, cmdLine* second_line);

int pipe_check(char* line) {
    for(int i=0;i<strlen(line);i++){
        if(line[i]=='|'){
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {

    int exit_flag;
    cmdLine* curr_line;

    printf("%s\n", "myshell is running...");

    while(1) {
        char user_line[BUF_SIZE];
        char path[BUF_SIZE];

        // gets and prints working path
        getcwd(path, BUF_SIZE);
        printf("myshell~%s ", path);

        // gets a line from user
        fgets(user_line, BUF_SIZE, stdin);

        // if "enter" then go to begin of loop
        if(user_line[0] == '\n')     
            continue;

        // if its a PIPE comment
        if(pipe_check(user_line)) {
            char* user_line1 = strtok(user_line, PIPE_TOKEN);
            char* user_line2 = strtok(NULL, PIPE_TOKEN);

            cmdLine* line1 = parseCmdLines(user_line1);
            cmdLine* line2 = parseCmdLines(user_line2);

            my_pipe(line1, line2);
            freeCmdLines(line1);
            freeCmdLines(line2);
            continue;
        }

        curr_line = parseCmdLines(user_line);

        // if "quit" then go out of loop
        if(cmp_arg0(curr_line, "quit"))
            break;

        // if cd
        if(cmp_arg0(curr_line, "cd")) {
                int status = chdir(curr_line->arguments[1]);
                if(status == -1)
                    perror("Failed to change directory: ");
        }
        else
            execute(curr_line);
        
        freeCmdLines(curr_line);
    }
    printf("%s\n", "exiting..");

return 0;
}