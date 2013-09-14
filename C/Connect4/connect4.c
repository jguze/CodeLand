#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define GRIDSIZE 6
#define PORT 2002
#define MAX_LINE 1000

char grid[GRIDSIZE][GRIDSIZE];
int current_player;
char EMPTY = '_';
char PLAYER_1_ICON = 'X';
char PLAYER_2_ICON = 'O';
int list_s;		// listening socket
int conn_s;		// connection socket
short int port;	// port number
struct sockaddr_in serv_addr;		// socket address structure
char buffer[MAX_LINE];	//character buffer

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
    return diag(coord, 1, 0);
}

int check_hor(tuple coord)
{
    int res = 0;
    res += diag(coord, 0, 1);
    res += diag(coord, 0, -1);
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

void network_setup()
{
	port = PORT;
	if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "SERVER: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(port);
	if ( bind(list_s, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		fprintf(stderr, "SERVER: Error calling bind().\n");
		exit(EXIT_FAILURE);
	}
	
	if ( listen(list_s, 10) < 0)
	{
		fprintf(stderr, "SERVER: Error calling listen().\n");
		exit(EXIT_FAILURE);		
	}
	
	/* Wait for a client to connect, and bind them to a player */
	conn_s = accept(list_s, (struct sockaddr*)NULL, NULL);
	read(conn_s, buffer, MAX_LINE);
	printf("CLIENT SAYS: %s\n", buffer);
	snprintf(buffer, sizeof(buffer), "Connected to Connect4 server. Playing against server player\n");
	write(conn_s, buffer, strlen(buffer));
}

void play()
{
	network_setup();
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
