/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// blank coordinates
int blankX;
int blankY;


// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }
    
    // blank tile starting coordinates
    blankX = d - 1;
    blankY = d - 1;
    
    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    //set max board value
    int tileValue = (d * d) - 1;
    
    // Initialize board in decending order
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d;j++){
            board[i][j] = tileValue;
            tileValue--;
        }
    }
    if(d % 2 == 0){
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
    }
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Print the board using a nested loop to iterate through the rows
    for(int i = 0; i < d;i++){
        for(int j = 0; j < d; j++){
            if(board[i][j] > 0){
                printf("| %2i |", board[i][j]);
            } else {
                printf("| __ |");
            }
            
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Check to see if move is valid
    //First, find tile
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            if(tile == board[i][j]){
                //Check position of tile relative to blank space
                if((i == blankY && (j == blankX - 1 || j == blankX + 1)) || (j == blankX && (i == blankY - 1 || i == blankY + 1))){
                    board[blankY][blankX] = board[i][j];
                    board[i][j] = 0;
                    blankY = i;
                    blankX = j;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int counter = 1;
    // Iterate through list to see if it matches winning configuration
    for(int i = 0; i < d;i++){
        for(int j = 0; j < d; j++){
            if(board[i][j] != counter){
                return false;
            }
            counter++;
            if(counter == d * d){
                return true;
            }
        }
    }
    return true;
}
