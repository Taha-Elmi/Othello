#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>

#define SIZE 8

// global variables
char board[SIZE][SIZE];
int white, black, free_cell = 64, placeable_cell = 4;

// functions
void gameloop();
void help();
void create_board();
void draw();
void insert(char player, char row, char column);
void reset();
int is_placeable(char player, int row, int column);
void show_placeable(char player);
void apply_changes(char player, char opponent, char row, char column);
void update_scores();
void display_scores();
void gameover();

void gameloop()
{
    create_board();

    int turn = 0;
    static int if_want = 3;
    // show placeable positions on board
    printf("Do you want us to show you placeable blocks? (yes: 1, no: 0) : ");
    scanf("%d", &if_want);
    while (if_want != 0 && if_want != 1){
        printf("Wrong answer. Just type 0 or 1: ");
        fflush(stdin);
        scanf("%d", &if_want);
    }
    if(if_want)
        show_placeable('W');

    while(free_cell != 0 && placeable_cell != 0)
    {
        system("cls");

        //display scores
        update_scores();
        display_scores();

        char player, oponent;
        player = (turn % 2) == 0 ? 'W' : 'B';
        oponent = player == 'W' ? 'B' : 'W';

        draw();

        printf("player %d its your turn\n",(turn % 2) + 1);

        char block[2];
        printf("choose a block (example: 1a): ");
        scanf("%s", &block);
        block[1] = toupper(block[1]);
        oponent = player == 'W' ? 'B' : 'W';

        // if the cell is empty and placeable insert
        if (is_placeable(player, (int)block[0] - 49, (int)block[1] - 65))
        {
            insert(player, block[0], block[1]);
            apply_changes(player, oponent, block[0], block[1]);
            turn++;
            reset();
            show_placeable(oponent); //to know if the game can be continued
        }
        else
        {
            printf("wrong move! try again.\n");
            Sleep(1500);
        }

        //if the players don't want the * marks
        if (if_want == 0)
            reset();

    }

    gameover();
}

/*
 * prints a help message for new players
 */
void help()
{
    char *text = "Hi! :)\nThe object of the game Othello is to win!\n\nThe game gets its name from Shakespeare's Othello, who kept changing\nfrom bad to good and back again --from white to black, back and forth.\nThere are two players: white and black. White goes first, by placing\na piece in some empty square(do this by typing the block you want). \nHowever, you can't place a white piece in just any square, \nbut only in one that satisfies the following condition:\n\n     In at least one direction (vertical, horizontal, or diagonal) emanating\n     from the square, there is a sequence of one or more black pieces\n     followed by a white piece.\n\nWhen a white piece is placed in such a square, all the black pieces\nmentioned in this condition become white --in all directions. Try it!\n\nAfter a white piece is placed, the other player places a black piece in the\nsame manner (with the roles of white and black interchanged). Then it's\nwhite's turn again, an so it goes.\n\nA player who cannot place a piece loses their turn. If no player can play,\nthe game is over, and the player with the most pieces wins.\n\n\tHave fun! :)))\n";

    printf("%s\npress any key to start",text);
    getch();
    system("cls");
}

void create_board()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = '-';
        }
    }

    insert('W', '4', 'D');
    insert('W', '5', 'E');
    insert('B', '4', 'E');
    insert('B', '5', 'D');
}

void draw()
{
    // row names display
    printf("   ");
    for (int i = 0; i < SIZE; i++)
    {
        printf(" %c ", i+65);
    }
    printf("\n");

    // board display
    for (int i = 0; i < SIZE; i++)
    {
        // column names display
        printf(" %i ",i+1);
        for (int j = 0; j < SIZE; j++)
        {
            printf(" %c ", board[i][j]);
        }
        printf("\n");
    }
}

void insert(char player, char row, char column)
{
    // change ascii code to places in matrix
    // its 49 and not 48 becaus arrays start with 0 not 1
    board[row - 49][column - 65] = player;
}


/*
 * return true if the given block is near other B/W blocks
 * else return false
 */
int is_placeable(char player, int row, int column)
{
    char opponent = player=='W' ? 'B' : 'W';

    int i = row, j = column;
    int x = i, y = j;

    if (board[x][y] == 'W' || board[x][y] == 'B')
        return 0;

    if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == opponent)
    {
        i--;
        j--;
        while(i > 0 && j > 0 && board[i][j] == opponent)
        {
            i--;
            j--;
        }
        if(i >= 0 && j >= 0 && board[i][j] == player)
            return 1;
    }

    i = x;
    j = y;

    if(i - 1 >= 0 && board[i - 1][j] == opponent){
        i--;
        while(i > 0 && board[i][j] == opponent)
            i--;
        if(i >= 0 && board[i][j] == player)
            return 1;
    }

    i = x;

    if(i - 1 >= 0 && j + 1 <= 7 && board[i - 1][j + 1] == opponent){
        i--;
        j++;
        while(i > 0 && j < 7 && board[i][j] == opponent)
        {
            i--;
            j++;
        }
        if(i >= 0 && j <= 7 && board[i][j] == player)
            return 1;
    }

    i = x;
    j = y;

    if(j - 1 >= 0 && board[i][j - 1] == opponent){
        j--;
        while(j > 0 && board[i][j] == opponent)
            j--;
        if(j >= 0 && board[i][j] == player)
            return 1;
    }

    j = y;

    if(j + 1 <= 7 && board[i][j + 1] == opponent){
        j++;
        while(j < 7 && board[i][j] == opponent)
            j++;
        if(j <= 7 && board[i][j] == player)
            return 1;
    }

    j = y;

    if(i + 1 <= 7 && j - 1 >= 0 && board[i + 1][j - 1] == opponent){
        i++;
        j--;
        while(i < 7 && j > 0 && board[i][j] == opponent)
        {
            i++;
            j--;
        }
        if(i <= 7 && j >= 0 && board[i][j] == player)
            return 1;
    }

    i = x;
    j = y;

    if(i + 1 <= 7 && board[i + 1][j] == opponent){
        i++;
        while(i < 7 && board[i][j] == opponent)
            i++;
        if(i <= 7 && board[i][j] == player)
            return 1;
    }

    i = x;
    j = y;


    if(i + 1 <= 7 && j + 1 <= 7 && board[i + 1][j + 1] == opponent){
        i++;
        j++;
        while(i < 7 && j < 7 && board[i][j] == opponent)
        {
            i++;
            j++;
        }
        if(i <= 7 && j <= 7 && board[i][j] == player)
            return 1;
    }

    return 0;
}

/*
 * draw an '*' in places where you can have your next move
 */
void show_placeable(char player)
{
    placeable_cell = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (is_placeable(player, i, j)) {
                board[i][j] = '*';
                placeable_cell++;
            }
        }
    }
}

void reset()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == '*')
            {
                board[i][j] = '-';
            }
        }
    }
}

/*
 * after inserting a point for your next move
 * this function apply changes in the game
 * and flip color of peaces which have the condition
 */
void apply_changes(char player, char opponent, char row, char column)
{
    int i = row - 49, j = column-65;
    int x = i, y = j;

    if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] == opponent)
    {
        i--;
        j--;
        while(i > 0 && j > 0 && board[i][j] == opponent)
        {
            i--;
            j--;
        }
        if(i >= 0 && j >= 0 && board[i][j] == player)
        {
            while(i != x - 1 && j != y - 1)
                board[++i][++j]=player;
        }
    }

    i = x;
    j = y;

    if(i - 1 >= 0 && board[i - 1][j] == opponent){
        i--;
        while(i > 0 && board[i][j] == opponent)
            i--;
        if(i >= 0 && board[i][j] == player)
        {
            while(i != x - 1)
                board[++i][j]=player;
        }
    }

    i = x;

    if(i - 1 >= 0 && j + 1 <= 7 && board[i - 1][j + 1] == opponent){
        i--;
        j++;
        while(i > 0 && j < 7 && board[i][j] == opponent)
        {
            i--;
            j++;
        }
        if(i >= 0 && j <= 7 && board[i][j] == player)
        {
            while(i != x - 1 && j != y + 1)
                board[++i][--j] = player;
        }
    }

    i = x;
    j = y;

    if(j - 1 >= 0 && board[i][j - 1] == opponent){
        j--;
        while(j > 0 && board[i][j] == opponent)
            j--;
        if(j >= 0 && board[i][j] == player)
        {
            while(j != y - 1)
                board[i][++j] = player;
        }
    }

    j = y;

    if(j + 1 <= 7 && board[i][j + 1] == opponent){
        j++;
        while(j < 7 && board[i][j] == opponent)
            j++;
        if(j <= 7 && board[i][j] == player)
        {
            while(j != y + 1)
                board[i][--j] = player;
        }
    }

    j = y;

    if(i + 1 <= 7 && j - 1 >= 0 && board[i + 1][j - 1] == opponent){
        i++;
        j--;
        while(i < 7 && j > 0 && board[i][j] == opponent)
        {
            i++;
            j--;
        }
        if(i <= 7 && j >= 0 && board[i][j] == player)
        {
            while(i != x + 1 && j != y - 1)
                board[--i][++j] = player;
        }
    }

    i = x;
    j = y;

    if(i + 1 <= 7 && board[i + 1][j] == opponent){
        i++;
        while(i < 7 && board[i][j] == opponent)
            i++;
        if(i <= 7 && board[i][j] == player)
        {
            while(i != x + 1)
                board[--i][j] = player;
        }
    }

    i = x;
    j = y;


    if(i + 1 <= 7 && j + 1 <= 7 && board[i + 1][j + 1] == opponent){
        i++;
        j++;
        while(i < 7 && j < 7 && board[i][j] == opponent)
        {
            i++;
            j++;
        }
        if(i <= 7 && j <= 7 && board[i][j] == player)
        {
            while(i != x + 1 && j != y + 1)
                board[--i][--j] = player;
        }
    }

}

void update_scores()
{
    // reset scores
    black = 0, white = 0, free_cell = 64;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            switch (board[i][j]) {
                case 'W':
                    white++;
                    free_cell--;
                    break;
                case 'B':
                    black++;
                    free_cell--;
                    break;
            }
        }
    }


}

void display_scores()
{
    printf("      BLACK %d - WHITE %d\n", black, white);
}

void gameover()
{
    system("cls");
    draw();
    if (black > white)
    {
        printf("\nBLACK TEAM WINS!\n");
    }
    else if (black < white)
    {
        printf("\nWHITE TEAM WINS!\n");
    }
    else
    {
        printf("\nDRAW!\n");
    }
    update_scores();
    display_scores();

    printf("\nWell well well, we wish you had fun with the game :)\n\nDo you wanna play again? (yes: 1, no: 0)");

    int answer = 3; //to enter the loop
    while (answer != 1 && answer != 0) {
        fflush(stdin);
        scanf("%d", &answer);
        switch (answer) {
            case 1:
                gameloop();
                break;
            case 0:
                system("cls");
                printf("Ok, thanks for choosing us. Bye :)");
                Sleep(2000);
                break;
            default:
                printf("Wrong answer. Just type 0 or 1: ");
    }

    }
}

int main()
{
    help();
    gameloop();
    return 0;
}