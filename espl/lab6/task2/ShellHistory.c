#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "LineParser.h"
#include "ShellHistory.h"

#ifndef NULL
    #define NULL 0
#endif

#define FREE(X) if(X) free((void*)X)

HistoryArray *createHistoryArray() {
    /*creates array to save our history*/
	HistoryArray* hist_arr = (HistoryArray*)malloc(sizeof(HistoryArray));
	hist_arr->line_count = 0;
	return hist_arr;
}

char *printHelper(cmdLine *line) {
    /*saves our command line as string so we can print it to the 
    terminal as string when printing our history*/
	char entire_line[2048];
	int index_of_line = 0;
	int curr_index = 0;
	
	while(curr_index < line->argCount) {
        int i;
		char *current_str = line->arguments[curr_index];
		int arg_size = strlen(current_str);	
		for(i = 0; i < arg_size; i++) {
			entire_line[index_of_line] = line->arguments[curr_index][i];
			index_of_line++;
		}
		entire_line[index_of_line] = ' ';
        curr_index++;
		index_of_line++;
	}
	index_of_line--;
	entire_line[index_of_line] = '\0';
	char *output_str = entire_line;
	
	return output_str;
}

void showHistory(HistoryArray *hist_arr) {
    /*prints the history to the terminal if given history flag*/
    int i;
	int hist_size = hist_arr->line_count;
	for(i = 0; i < hist_size; i++) {
		char *str_line = printHelper(hist_arr->history_array[i]);
		printf(" %d  %s\n", i, str_line);
	}
}



int addLine(HistoryArray *hist_arr, cmdLine *line) {
    /*adds a new line to our history array to save the history*/
	int hist_size = hist_arr->line_count;
	if(hist_size != 256 - 1) {
		hist_arr->history_array[hist_size] = copy_cmd_line(line);
		hist_arr->line_count++;
		return 0;
	}
	return -1;
}


void deleteHistory(HistoryArray *hist_arr) {
    /*frees the history array from the memory preventing memory leak*/
    int i;
	int hist_size = hist_arr->line_count;
	for(i = 0; i < hist_size; i++) {
		freeCmdLines(hist_arr->history_array[i]);
	}
	FREE(hist_arr);
}

