#include <cstdio>
#include <Windows.h>
#include "board.h"
#include "constants.h"

Point NewPoint(int x, int y)
{
	Point TempPoint;
	TempPoint.x = x;
	TempPoint.y = y;
	return TempPoint;
}


//Initilizes the Board
void CreateBoard(Board& Board)
{
	Board.Rows = ROWS; // 5
	Board.Columns = COLUMNS; // 9
	for (int i = 0; i < Board.Rows; i++)
	{
		for (int j = 0; j < Board.Columns; j++)
		{
			Board.Matrix[i][j] = EMPTY; //0
		}
	}
}

//Stores the initial postions of the BLACK and WHITE stones in the matrix
void InitializeBoard(Board& Board)
{
	int x = 0;
	int y = 0;

	while (y < Board.Rows)
	{
		x = 0;
		while (x < Board.Columns)
		{
			if (y < 2)
			{
				Board.Matrix[y][x] = BLACK;
			}
			else if (y > 2)
			{
				Board.Matrix[y][x] = WHITE;
			}
			else
			{
				if (x < 4)
				{
					if (x % 2 == 0)
					{
						Board.Matrix[y][x] = WHITE;
					}
					else
					{
						Board.Matrix[y][x] = BLACK;
					}
				}
				else if (x > 4)
				{
					if (x % 2 == 0)
					{
						Board.Matrix[y][x] = BLACK;
					}
					else
					{
						Board.Matrix[y][x] = WHITE;
					}
				}
				else
				{
					Board.Matrix[y][x] = EMPTY;
				}
			}
			x++;
		}
		y++;
	}
}

void gotoxy(int column, int line)
{
	coord coord;
	coord.x = column;
	coord.y = line;
	setconsolecursorposition(
		getstdhandle(std_output_handle),
		coord
	);
	//printf("\033[%d;%dH", column, line);
	//printf("\t\t\t\t\t\t\t\t");
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
	//printf("\033[%d;%dH", column, line);
}

//Shows the Board on the Screen
void Show(Board Board)
{
	Sleep(SLEEP_MS);
	gotoxy(0, 0);

	printf("\n");
	printf(" ");
	for (int i = 0; i < Board.Columns; i++) 
		printf(" %d", i);
	printf("\n");
	

	for (int i = 0; i < Board.Rows; i++)
	{
		
		if (i > 0)
		{	
			printf("  ");
			for (int j = 0; j < Board.Columns; j++)
			{
				printf("|");
				if (j < Board.Columns - 1)
				{
					if ((i + j) % 2 == 0)
					{
						printf("/");
					}
					else
					{
						printf("\\");
					}
				}
			}
			printf("\n");
		}
		printf("%d ", i);
		for (int j = 0; j < Board.Columns; j++)
		{	
			switch (Board.Matrix[i][j])
			{
			case EMPTY:
				printf(".");
				break;
			case DELETE_APPROACHED_BLACK:
			case DELETE_WITHDRAWED_BLACK:
			case BLACK:
				printf("#");
				break;
			case DELETE_APPROACHED_WHITE:
			case DELETE_WITHDRAWED_WHITE:
			case WHITE:
				printf("O");
				break;
			case SELECTED_BLACK:
			case SELECTED_WHITE:
				printf("*");
				break;
			}
			if (j < Board.Columns - 1)
				printf("-");
		}
		printf("\n");
	}
	printf("\n");

}

//Returns the color of Stone at (x, y)
int GetStonecolor(Board Board, int x, int y)
{
	return Board.Matrix[y][x];

}

//Returns true if Stone exists at (x, y)
bool StoneExists(Board Board, int x, int y)
{
	if (IsOn(Board, x, y))
	{
		if (Board.Matrix[y][x] != EMPTY)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

//Unselects all stones on the board
void UnselectALL(Board& Board)
{
	for (int i = 0; i < Board.Rows; i++)
	{
		for (int j = 0; j < Board.Columns; j++)
		{
			if (Board.Matrix[i][j] == SELECTED_BLACK)
				Board.Matrix[i][j] = BLACK;
			else if (Board.Matrix[i][j] == SELECTED_WHITE)
				Board.Matrix[i][j] = WHITE;
		}
	}
}
//Gets the Postion selected stone
Point GetSelectedStone(Board Board, int& color)
{
	Point Pos;
	Pos.x = -1;
	Pos.y = -1;
	color = -1;

	for (int i = 0; i < Board.Rows; i++)
	{
		for (int j = 0; j < Board.Columns; j++)
		{
			if (Board.Matrix[i][j] == SELECTED_BLACK)
			{
				Pos.x = j;
				Pos.y = i;
				color = BLACK;
				return Pos;
			}
			else if (Board.Matrix[i][j] == SELECTED_WHITE)
			{
				Pos.x = j;
				Pos.y = i;
				color = WHITE;
				return Pos;
			}
		}
	}
	return Pos;

}


//return BLACK, White of Empty for winner
int CheckWinner(Board Board)
{
	int x = 0;
	int y = 0;
	int c = EMPTY;

	while (x < Board.Columns)
	{
		while (y < Board.Rows)
		{
			if (Board.Matrix[y][x] != EMPTY && c == EMPTY)
			{
				c = Board.Matrix[y][x];
			}
			else if (Board.Matrix[y][x] != EMPTY && Board.Matrix[y][x] != c)
			{
				return EMPTY;
			}
			y++;
		}
		y = 0;
		x++;
	}
	return c;
}


//returns if the row or col is on the board or not
bool IsOn(Board Board, int xx, int yy)
{
	if (xx >= 0 && xx < Board.Columns && yy >= 0 && yy < Board.Rows)
		return true;
	return false;
}

//Copies OldBoard to New Board
void Copy(Board OldBoard, Board& NewBoard)
{
	NewBoard = OldBoard;
}
