#ifndef GOLFUNC
#define GOLFUNC

char *strtok(char *str, const char *delim);
void printBoardState(char **golboard, int nrow, int ncol, char cell);
void initBoard(char boardInitState, size_t nRows, size_t nCols);
#endif