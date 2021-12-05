typedef struct HistoryArray
{
	struct cmdLine *history_array[256];
	int line_count;	
} HistoryArray;

HistoryArray *createHistoryArray();

char *printHelper(cmdLine *line);

void showHistory(HistoryArray *hist_arr);

cmdLine *copyLine(cmdLine *line);

int addLine(HistoryArray *hist_arr, cmdLine *line);

void deleteHistory(HistoryArray *hist_arr);