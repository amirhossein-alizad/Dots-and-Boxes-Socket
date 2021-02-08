#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
char horizental[5][5];
char vertical[5][5];
char names[5][5];
int points[4] = {0};
void init()
{
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			vertical[i][j] = ' ';
			horizental[i][j] = ' ';
			names[i][j] = ' ';
		}
	}
}
void sigHandler(int signum)
{
	write(1, "YOU RAN OUT OF TIME!\n",22);
}

void winner(int gp)
{
	int idx;
	int max = 0, sum = 0;
	for(int i = 0; i < gp; i++)
	{
		sum += points[i];
		if (points[i] > max)
		{
			max = points[i];
			idx = i;
		}
	}
	if (sum < (gp + 1) * (gp + 1))
	{
		write(1, "NO WINNER YET\n", 15);
		return;
	}
	if(idx % gp == 0)
	{
		write(1, "PLAYER A WINS!\n", 16);
		exit(EXIT_SUCCESS);
	}
	if(idx % gp == 1)
	{
		write(1, "PLAYER B WINS!\n", 16);
		exit(EXIT_SUCCESS);
	}
	if(idx % gp == 2)
	{
		write(1, "PLAYER C WINS!\n", 16);
		exit(EXIT_SUCCESS);
	}
	if(idx % gp == 3)
	{
		write(1, "PLAYER D WINS!\n", 16);
		exit(EXIT_SUCCESS);
	}
}
void shapes(char * buff)
{
	int coordinate1 = atoi(&buff[0]);
	int coordinate2 = atoi(&buff[2]);
	int coordinate3 = atoi(&buff[4]);
	if (coordinate1 == 0)
	{
		horizental[coordinate2 - 1][coordinate3 - 1] = '-';
	}
	else
	{
		vertical[coordinate2 - 1][coordinate3 - 1] = '|';
	}
}

int name(char * buff, int * game_turn, int turn, int gp)
{
	int coordinate1 = atoi(&buff[0]);
	int coordinate2 = atoi(&buff[2]);
	int coordinate3 = atoi(&buff[4]);
	int flag = 0;
	if (coordinate1 == 0)
	{
		if (horizental[coordinate2][coordinate3 - 1] == '-' && vertical[coordinate2 - 1][coordinate3 - 1] == '|' && vertical[coordinate2 - 1][coordinate3] == '|')
		{
			if (*game_turn % gp == 3)
			{
				names[coordinate2 - 1][coordinate3 - 1] = 'D';
			}
			else if (*game_turn % gp == 2)
			{
				names[coordinate2 - 1][coordinate3 - 1] = 'C';
			}
			else if (*game_turn % gp == 1)
			{
				names[coordinate2 - 1][coordinate3 - 1] = 'B';
			}
			else
			{
				names[coordinate2 - 1][coordinate3 - 1] = 'A';
			}
			flag = 1;
			points[*game_turn % gp] += 1;
		}
		if (horizental[coordinate2 - 2][coordinate3 - 1] == '-' && vertical[coordinate2 - 2][coordinate3 - 1] == '|' && vertical[coordinate2 - 2][coordinate3] == '|')
		{
			if (*game_turn % gp == 3)
			{
				names[coordinate2 - 2][coordinate3 - 1] = 'D';
			}
			else if (*game_turn % gp == 2)
			{
				names[coordinate2 - 2][coordinate3 - 1] = 'C';
			}
			else if (*game_turn % gp == 1)
			{
				names[coordinate2 - 2][coordinate3 - 1] = 'B';
			}
			else
			{
				names[coordinate2 - 2][coordinate3 - 1] = 'A';
			}
			flag = 1;
			points[*game_turn % gp] += 1;
		}
	}
	if (coordinate1 == 1)
	{
		if (vertical[coordinate2 - 1][coordinate3] == '|' && horizental[coordinate2 - 1][coordinate3 - 1] == '-' && horizental[coordinate2][coordinate3 - 1] == '-')
		{
			if (*game_turn % gp == 3)
			{
				names[coordinate2 - 1][coordinate3 - 1] = 'D';
			}
			else if (*game_turn % gp == 2)
			{
				names[coordinate2 - 1][coordinate3 - 1] = 'C';
			}
			else if (*game_turn % gp == 1)
			{
				names[coordinate2 - 1][coordinate3 - 1] = 'B';
			}
			else
			{
				names[coordinate2 - 1][coordinate3 - 1] = 'A';
			}
			flag = 1;
			points[*game_turn % gp] += 1;
		}
		if (vertical[coordinate2 - 1][coordinate3 - 2] == '|' && horizental[coordinate2 - 1][coordinate3 - 2] == '-' && horizental[coordinate2][coordinate3 - 2] == '-')
		{
			if (*game_turn % gp == 3)
			{
				names[coordinate2 - 1][coordinate3 - 2] = 'D';
			}
			else if (*game_turn % gp == 2)
			{
				names[coordinate2 - 1][coordinate3 - 2] = 'C';
			}
			else if (*game_turn % gp == 1)
			{
				names[coordinate2 - 1][coordinate3 - 2] = 'B';
			}
			else
			{
				names[coordinate2 - 1][coordinate3 - 2] = 'A';
			}
			flag = 1;
			points[*game_turn % gp] += 1;
		}
	}
	return flag;
}
//
void newmap(int n)
{
	int counter1, counter2, counter3;
	for (counter1 = 0;counter1 <= n;counter1++)
	{
		for (counter2 = 0; counter2 < n ; counter2++)
		{
			write(1, "* ", 2);
			if(horizental[counter1][counter2] == '-')
			{
				write(1, &horizental[counter1][counter2], 1);
				write(1, &horizental[counter1][counter2], 1);
			}
			else
			{
				write(1, "  ", 2);
			}
			write(1, " ", 1);
		}
		write(1, "*\n", 2);

		for (counter3 = 0;counter3 <=n;counter3++)
		{
			write(1, &vertical[counter1][counter3], 1);
			write(1, "  ", 2);
			write(1, &names[counter1][counter3], 1);
			write(1, " ", 1);
		}
		write(1, "\n", 1);
	}
}
