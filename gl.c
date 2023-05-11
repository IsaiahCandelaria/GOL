/**
 * @file gl.c
 * @author Isaiah Candelaria
 * @brief Game of Life
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gl.h"

char boardInitState = 'r';
int i, j;
int counter;
char **board;
char **blankBoard;
int evolutions = 15;
int timeBetweenEvolutions = 200000;
int randomSeed = 50;
char cellAscii = 'x';
char file_n[100];
int x = 20;
int y = 20;
char*** trash;
int size = 0;

/**
 *@brief Prints the current state of the game of life board to the console
 *@param golboard The game of life board to be printed
 *@param nrow The number of rows in the board
 *@param ncol The number of columns in the board
 *@param cell The character used to represent a live cell in the board
 *The function prints the current state of the game of life board to the console using
 *the specified character to represent a live cell and an empty space to represent a dead cell.
 *The cursor is set to the top left corner of the console before printing the board, allowing
 *the board to be printed over the previous board.
**/
void printBoardState(char **golboard, int nrow, int ncol, char cell) 
{
        printf("\033[H"); // Sets cursor to homestate
        char boardtranslate[] = {' ', cell};
        char buff[2*(nrow * ncol) + nrow + 1];
        char *buffp = buff;
        int k = 0;
        for (int i = 0; i < nrow; ++i)
        {
                for (int j = 0; j < ncol; ++j)
        {
                *(buffp + k++) = *(boardtranslate + *(*(golboard + i) + j));
                *(buffp + k++) = ' ';
        }
        *(buffp + k++) = '\n';
    }
    *(buffp + k) = 0;
    printf("%s", buff);
}
/**
 * @brief Initializes the board based on the input parameters.
 * 
 * @param nRows An integer variable indicating the number of rows in the board
 * @param nCols  An integer variable indicating the number of columns in the board.
 */
//attemt to free board
void freeBoard(size_t nRows)
{
    if (board != NULL)
    {
        for (i = 0; i < nRows; i++)
        {
            free(board[i]);
        }
        free(board);
        board = NULL;  //set the pointer to NULL after freeing the memory
    }
}
//initialize the board
void initBoard(char boardInitState, size_t nRows, size_t nCols)
{
    //printf("Hereeeeeeeee");
        if (boardInitState == 'f')
{
    //printf("Hereeeeeeeee");
        //free(board);
        freeBoard(nRows);
        board = malloc(nRows * sizeof(char *));
        for (i = 0; i < nRows; i++)
        {
                //free(board[i]);
                board[i] = malloc(nCols * sizeof(char));
        if (board[i] == NULL) 
        {
                fprintf(stderr, "Error: failed to allocate memory for board row %d.\n", i);
                exit(1);
        }
        for (j = 0; j < nCols; j++)
        {
            board[i][j] = 0;
        }
        }
        FILE *X = fopen(file_n, "r");
        if (X == NULL) 
        {
            fprintf(stderr, "Error: failed to open file '%s'.\n", file_n);
            exit(1);
        }
        if (X != NULL) 
        {
                char Read[64]; // increase size of Read array
                //int x = nRows / 2; // Middle coordinates
                //int y = nCols / 2;
                while (fgets(Read, 64, X) != NULL) 
                {
                //printf("%s", Read);
                int xxx = atoi(strtok(Read, " "));
                 //printf("%d", xxx);
                int yyy = atoi(strtok(NULL, " "));
                //printf("%d", yyy);
            
                if (xxx+x >= 0 && xxx+x < nRows && yyy+y >= 0 && yyy+y < nCols) 
                {
                        board[xxx+x][yyy+y] = 1;
                }
                }
                fclose(X);
                }
         

}
 
    
if (boardInitState == 'b')
{
        //freeBoard(nRows);

        blankBoard = malloc(nRows * sizeof(char *));
        for (i = 0; i < nRows; i++)
        {
                blankBoard[i] = malloc(nCols * sizeof(char));
                for (j = 0; j < nCols; j++)
                {
                        blankBoard[i][j] = 0;
                }
        }
        
}
    // board with random cells initiated
    else if (boardInitState == 'r')
    {
        //freeBoard(nRows);
        srand(randomSeed);
        board = malloc(nRows * sizeof(char *));
        for (i = 0; i < nRows; i++)
        {
            board[i] = malloc(nCols * sizeof(char));
            for (j = 0; j < nCols; j++)
            {
                x = rand() % 1234;
                if (x == 1)
                {
                    board[i][j] = 1;
                }
                else
                {
                    board[i][j] = 0;
                }
            }
        }
    }
        // if(board != NULL)
        // {
        // trash = realloc(trash, sizeof(char**) * (size + 2));
        // trash[size++] = board;
        // trash[size++] = blankBoard;
        // }
        
        // trash[size - 1] = board;
        // size++;
        // realloc(trash, sizeof(char**) * size);

        
        // trash[size - 1] = blankBoard;
        // size++;
        // realloc(trash, sizeof(char**) * size);
    
}
/**
 * @brief 
 * @param i 
 * @param j 
 * @param nRows 
 * @param nCols 
 * @return int 
 */
int _countCellNeighbours(int i, int j, size_t nRows, size_t nCols) //rows,  collums
{
    int counter = 0;

    char* currRow = board[i];
    char* prevRow = (i > 0) ? board[i - 1] : NULL;
    char* nextRow = (i < nRows - 1) ? board[i + 1] : NULL;

    if (prevRow != NULL)
    {
        if (*(prevRow + j) == 1)
        {
            counter += 1;
        }
        if (j > 0 && *(prevRow + j - 1) == 1)
        {
            counter += 1;
        }
        if (j < nCols - 1 && *(prevRow + j + 1) == 1)
        {
            counter += 1;
        }
    }

    if (nextRow != NULL)
    {
        if (*(nextRow + j) == 1)
        {
            counter += 1;
        }
        if (j > 0 && *(nextRow + j - 1) == 1)
        {
            counter += 1;
        }
        if (j < nCols - 1 && *(nextRow + j + 1) == 1)
        {
            counter += 1;
        }
    }

    if (j > 0 && *(currRow + j - 1) == 1)
    {
        counter += 1;
    }
    if (j < nCols - 1 && *(currRow + j + 1) == 1)
    {
        counter += 1;
    }

    return counter;
}

void _evolveCell(int i, int j, int counter)
{

    if (board[i][j] == 1)
    {
        if (counter < 2)
        {
            blankBoard[i][j] = 0;//kills
        }
        else if (counter == 2 || counter == 3)
        {
            blankBoard[i][j] = 1;//born
        }
        else
        {
            blankBoard[i][j] = 0;
        }
    }

    else
    {
        if (counter == 3)
        {
            blankBoard[i][j] = 1;
        }
    }
}

void evolveBoard(size_t nRows, size_t nCols, char *type)//parameter is to enter diffrent modes like hedge Klein and Torus
{
    for (i = 0; i < nRows; i++)
    {
        for (j = 0; j < nCols; j++)
        {
            counter = _countCellNeighbours(i, j, nRows, nCols);
            _evolveCell(i, j, counter);
        }
    }
    if(!strcmp(type, "hedge"))//hedge
    {
        for (i = 0; i < nRows; i++)
        {
            for (j = 0; j < nCols; j++)
            {                                  
                                                                                                                              
                if(i == 0 || i == nRows - 1 || j == 0 || j == nCols - 1)
                {
                    blankBoard[i][j] = 0;
                }
            }
        }
    }

    // if(!strcmp(type, "torus"))//Torus
    // {
    //     for (i = 0; i < nRows; i++)
    // {
    //     for (j = 0; j < nCols; j++)
    //     {
    //         blankBoard[i][j] = (i - 1) % nRows;
    //         blankBoard[i][j] = (j - 1) % nCols;
            
    //     }
    // }
    // }  
}

int updateOldBoard(size_t nRows, size_t nCols)
{
    int max_iter = nRows * nCols;
    int count = 0;
    for (i = 0; i < nRows; i++)
    {
        for (j = 0; j < nCols; j++)
        {
            if(board[i][j] == blankBoard[i][j])
            {
                count++;
            }
            board[i][j] = blankBoard[i][j];
        }
    }

    if(count == max_iter)
    {
        return 1;
    }


    return 0;
}
/**
 *Usage: life -w <width> -h <height> -f <filename> -o <x,y> -e <edge_type>
 * Options:
 *-w <width> Set the width of the screen. Default value is 40.
 *-h <height> Set the height of the screen. Default value is 40.
 *-f <filename> Specify the name of the file containing the life pattern in 1.06 format.
 *-o <x,y> Set the initial x and y coordinates of the pattern. No space between the x and y.
 *-e <edge_type> Set the type of edge. Values are hedge.
 *-H Display usage information and a brief description of each option.
 */
void print_help() 
{
        printf("Usage: life -w <width> -h <height> -f <filename> -o <x,y> -e <edge_type>\n");
        printf("\nOptions:\n");
        printf("-w <width>         Set the width of the screen. Default value is 40.\n");
        printf("-h <height>        Set the height of the screen. Default value is 40.\n");
        printf("-f <filename>      Specify the name of the file containing the life pattern in 1.06 format.\n");
        printf("-o <x,y>           Set the initial x and y coordinates of the pattern. No space between the x and y.\n");
        printf("-e <edge_type>     Set the type of edge. Values are hedge.\n");
        printf("-H                 Display usage information and a brief description of each option.\n");
}
/**
 *@brief The main function of the Game of Life program.
 *@param argc An integer representing the number of command line arguments.
 *@param argv An array of strings representing the command line arguments.
 *@return exit
 * The main function of the Game of Life program accepts
 * command line arguments for the width, height, edge type, 
 * file name, and initial coordinates of the board. 
 * It initializes the board and evolves it based on the 
 * given edge type until a stable state is reached. 
 * The function prints the board state after each evolution. 
 * The function returns 0 if successful, 1 if there is an error 
 * in the input arguments, and 2 if there is an error in the board initialization.
**/
int main(int argc, char *argv[])
{
    int opt;
    printf("\033[?25l"); // makes cursor invisible
    size_t nCols = 40, nRows = 40;
    char edge_type[10];
    //printf("made it");
    size = 1;
    trash = malloc(sizeof(char**));

    while ((opt = getopt(argc, argv, "w:h:e:f:o:H")) != -1) 
{
    switch (opt) {
        case 'w':
            if (sscanf(optarg, "%ld", &nCols) != 1 || nCols <= 0) {
                fprintf(stderr, "Invalid width argument: %s\n", optarg);
                return 1;
            }
            break;
        case 'h':
            if (sscanf(optarg, "%ld", &nRows) != 1 || nRows <= 0) {
                fprintf(stderr, "Invalid height argument: %s\n", optarg);
                return 1;
            }
            break;
        case 'e':
            if (sscanf(optarg, "%s", edge_type) != 1) {
                fprintf(stderr, "Invalid edge type argument: %s\n", optarg);
                return 1;
            }
            break;
        case 'f':
            strcpy(file_n, optarg);
            boardInitState = 'f';
            break;
        case 'o':
            if (sscanf(optarg, "%d,%d", &x,&y) != 2) {
                fprintf(stderr, "Invalid coordinates argument: %s\n", optarg);
                return 1;
            }
            break;
        case 'H':
            print_help();
            return 0;
        default:
            print_help();
            return 1;
    }
}
        /**
         * @brief Error checking
         * 
         */
        if (nCols <= 0 || nCols > 500)
        {
                nCols = 40;
        }
        if (nRows <= 0 || nRows > 500) 
        {
                nRows = 40;
        }
        
        if (strlen(edge_type) == 0) 
        {
                strcpy(edge_type, "hedge");//defult type
        }
        if (nRows <= 0 || nCols <= 0) 
        {
                print_help();
                return 1;
        }
        //initBoard('b');
        //alloc_board(nRows, nCols);
        initBoard(boardInitState, nRows, nCols);
        int run = 0;

        while(run == 0)
        {
                //printf("made it");
                usleep(timeBetweenEvolutions);
                initBoard('b',nRows, nCols);
                evolveBoard(nRows, nCols, edge_type);
                run = updateOldBoard(nRows, nCols);
                printBoardState(board, nRows, nCols, cellAscii);
        }
        //printf("made it");
        printf("\033c"); 
        printf("\033[?25h"); // makes cursor visible
        
//       for (int i = 0; i < size; i++)
// {
//     for (int j = 0; j < nCols; j++)
//     {
//         free(trash[i][j]);
//     }
//     free(trash[i]);
// }
freeBoard(nRows);
return 0;

}
