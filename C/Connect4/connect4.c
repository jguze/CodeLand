#include <stdio.h>
#define GRIDSIZE 6

char grid[GRIDSIZE][GRIDSIZE];
int current_player;
char EMPTY = '_';
char PLAYER_1_ICON = 'X';
char PLAYER_2_ICON = 'O';

typedef struct 
{
    int x;
    int y;
} tuple;

void display()
{
	int i, j;
    printf("COLUMNS\n\n");
    for (i = 0; i < GRIDSIZE; i++)
    {
        printf("| %d ", i);
    }
    printf("|\n\n");
    for (i = 0; i < GRIDSIZE; i++)
    {
        for (j = 0; j < GRIDSIZE; j++)
        {
            printf("| %c ", grid[i][j]);
        }
        printf("|\n\n");
    }
}

char player_icon()
{
    if (current_player == 0)
    {
        return PLAYER_1_ICON;
    }
    else {
        return PLAYER_2_ICON;
    }
}

tuple place_item(int col, int * error)
{
    int i;
    tuple result;
    *error = check_bounds(col, 0);
    if (*error == 0)
        return result;
    for (i = GRIDSIZE - 1; i >= 0; i--)
    {
        if (grid[i][col] == EMPTY)
        {
            grid[i][col] = player_icon();
            result.x = i;
            result.y = col;
            return result;
        }
    }
    *error = 0;
    return result;
}

void reset()
{
	int i, j;
    for (i = 0; i < GRIDSIZE; i++)
    {
        for (j = 0; j < GRIDSIZE; j++)
        {
            grid[i][j] = '_';
        }
    }
}

int check_bounds(int x, int y)
{
    if (x < 0 || x > GRIDSIZE)
        return 0;
    if (y < 0 || y > GRIDSIZE)
        return 0;
    return 1;
}

int check_validity(tuple coord, int x, int y)
{
        if(!check_bounds(coord.x + x, coord.y + y))
        {
            return 0;
        }   
        if (grid[coord.x + x][coord.y + y] != player_icon())
        {
            return 0;
        }
    return 1;
}

int diag(tuple coord, int signx, int signy)
{
    int i;
    for (i = 1; i < 4; i++)
    {
        if (!check_validity(coord, i * signx, i * signy))
            return 0;
    }
    return 1;
}

int check_vert(tuple coord)
{
    return diag(coord, 0, -1);
}

int check_hor(tuple coord)
{
    int res = 0;
    res += diag(coord, 1, 0);
    res += diag(coord, -1, 0);
    if (res > 0)
        return 1;
    return 0;
}

int check_diag(tuple coord)
{
    int res = 0;
    res += diag(coord, 1, 1);
    res += diag(coord, 1, -1);
    res += diag(coord, -1, 1);
    res += diag(coord, 1, 1);
    if (res > 0)
        return 1;
    return 0;
        
}

int has_won(tuple coord)
{
    int res = 0;
    res += check_vert(coord);
    res += check_hor(coord);
    res += check_diag(coord);
    if (res > 0)
        return 1;
    return 0;
}

void play()
{
	reset();
    current_player = 0;
    int col;
    int error = 1;
    tuple placed;
    while(1)
    {
        display();
        printf("What column should Player %d place the block?\n\n", current_player + 1);
        scanf("%d", &col);
        placed = place_item(col, &error);
        if (error == 0)
        {
            error = 1;
            printf("Invalid move. Try again\n\n");
            continue;
        }
        if (has_won(placed))
        {
            break;
        }
        current_player = (current_player + 1) % 2;
    }
    display();
    printf("Player %d has won!\n", current_player + 1);
}

int main()
{
    play();
	return 0;
}
