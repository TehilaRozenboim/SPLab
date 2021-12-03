#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "LineParser.h"

void execute(cmdLine *cmdLine) {

    int result = fork();
    int iscmdErr ;
    char err[1024];
    if(result == -1){
        perror("fork failed");
        return 1;
    } else if (result == 0){
        iscmdErr = execvp(cmdLine->arguments[0], cmdLine->arguments);
        if (iscmdErr){
            perror(err);
            printf("%s", err);
        }
        _exit(iscmdErr);
    } else {
        if (cmdLine->blocking)
            waitpid(result, &iscmdErr, 0);
    }
    return iscmdErr;
}

int handel_input(char input[])
{
    if (strcmp(input, "quit\n") == 0){
            return 0;
    } else {
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

        getcwd(buff, PATH_MAX);
        printf("%s> ", buff);
        fgets(input, MAX_OF_LINES, stdin);
        inLoop = handel_input(input);
    }

    printf("\n");
    return 0;
}

