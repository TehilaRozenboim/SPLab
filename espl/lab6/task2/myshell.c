#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "LineParser.h"
#include "ShellHistory.h"

int execute(cmdLine *cmdLine){
    /*receives parsed line and invokes command using proper system call*/
    int result = fork();
    int iscmdErr;
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


int exec_chdir(cmdLine *line){
    int output;
    char error_string[1024];
    if(line->argCount == 1){
        output = chdir(getenv("HOME"));
    }
    else if (line->argCount == 2) {
        output = chdir(line->arguments[1]);
    }
    else{
        output = chdir("");
    }
    if(output) {
		perror(error_string);
        fprintf(stderr, "%s", error_string);
	}
return output;
}

long int convert_string_to_integer(char *history_num_string) {
    char *temp;
    long int history_num = strtol(history_num_string, &temp, 0);

    if (temp == history_num_string || *temp != '\0' || (history_num == 0 || history_num == 5))
        fprintf(stderr, "bash: !%s: event not found\n", history_num_string);
    return history_num;
}


int handle_history(cmdLine **line_pointer, HistoryArray *shell_history) {
	char history_num_buffer[10];
	char *history_num_string;
	long int long_history_num;

	history_num_string = strcpy(history_num_buffer, (*line_pointer)->arguments[0] + 1);
    long_history_num = convert_string_to_integer(history_num_string);

    if(long_history_num > shell_history->line_count - 1) {
        fprintf(stderr, "bash: !%ld: event not found\n", long_history_num);
        return -1;
    }
    free(*line_pointer);
    *line_pointer = copy_cmd_line(shell_history->history_array[long_history_num]);
    return 0;
}


int handle_input(char input[], HistoryArray *history_array){
    if (strcmp(input, "quit\n") == 0){
            return 0;
    }
    int result; 
    int history_exit;
    /*parse command line*/
    cmdLine *line = parseCmdLines(input);

    if(line->arguments[0][0] == '!') {
		history_exit = handle_history(&line, history_array);
		if(history_exit == -1) {
			return 1;
		}
	}
    addLine(history_array, line);
    if(strcmp(line->arguments[0], "cd") == 0){
        result = exec_chdir(line);
    }
    else if(strcmp(line->arguments[0], "history") == 0) {
		 showHistory(history_array);
         return 0;
	}
    else{
        result = execute(line);
    }
    freeCmdLines(line);
    return result;
    }
     

int main(int argc, char const *argv[]){
    int const MAX_OF_LINES = 2048;
    char input[MAX_OF_LINES];
    char buff[PATH_MAX];
    int inLoop = 1;
    HistoryArray *history_array = createHistoryArray();
    while (inLoop) {
        /*get current working directory, path name shouldn't exceed path_max*/
        getcwd(buff, PATH_MAX); 
        printf("%s> ", buff);
        /*read line from user*/
        fgets(input, MAX_OF_LINES, stdin);
        inLoop = !handle_input(input, history_array);
    }
    deleteHistory(history_array);
    printf("\n");
    return 0;
}
