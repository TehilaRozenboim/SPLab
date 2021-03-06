#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "LineParser.h"


int execute(cmdLine *cmdLine) { 
    /*receives parsed line and invokes command using proper system call*/
    int result = fork();
    int iscmdErr ;
    char err[1024];
    if(result == -1){ 
        /*display an error if fork fails*/
        perror("fork failed");
        return 1;
    } else if (result == 0){ /*child process*/
        iscmdErr = execvp(cmdLine->arguments[0], cmdLine->arguments);
        if (iscmdErr){
            /*display error if execv fails*/
            perror(err);
            printf("%s", err);
        }
        _exit(iscmdErr);
    } else {/*parent process*/
        if (cmdLine->blocking)
            /*task1b*/
            waitpid(result, &iscmdErr, 0);/*wait for child to exit*/
    }
    return iscmdErr;
}

int handle_input(char input[])
{
    if (strcmp(input, "quit\n") == 0){
            return 0;
    } 
    else {
    /*parse command line*/
    cmdLine *line = parseCmdLines(input);
    execute(line);
    freeCmdLines(line);
    }
    return 1;        
}

int main(int argc, char const *argv[]){
    int const MAX_OF_LINES = 2048;
    char input[MAX_OF_LINES];
    char buff[PATH_MAX];
    int inLoop = 1;
    
    while (inLoop) {
        /*get current working directory, path name shouldn't exceed path_max*/
        getcwd(buff, PATH_MAX); 
        printf("%s> ", buff);
        /*read line from user*/
        fgets(input, MAX_OF_LINES, stdin);
        inLoop = !handle_input(input);
    }

    printf("\n");
    return 0;
}
