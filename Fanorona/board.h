#ifndef _BOARD_H
#define _BOARD_H

struct Point
{
	int x;
	int y;
};

struct Board
{
	int Rows;					// 5
	int Columns;				// 9
	int Matrix[5][9];			//Stores the poisitons of various stones
	int Chance;
};

//create point
Point NewPoint(int x, int y);

//Initilizes the Board
void CreateBoard(Board& Board);

//Stores the initial postions of the BLACK and WHITE stones in the matrix
void InitializeBoard(Board& Board);

void gotoxy(int column, int line);

//Shows the Board on the Screen
void Show(Board Board);

//Returns the color of Stone at (x, y)
int GetStonecolor(Board Board, int x, int y);

//Returns the true if Stone exists at (x, y)
bool StoneExists(Board Board, int x, int y);

//Unselects all stones on the board
void UnselectALL(Board& Board);

//Gets the Postion selected stone
Point GetSelectedStone(Board Board, int& color);

//return BLACK, White of Empty for winner
int CheckWinner(Board Board);

//returns if the row or col is on the board or not
bool IsOn(Board Board, int xx, int yy);

//Copies OldBoard to New Board
void Copy(Board OldBoard, Board& NewBoard);

#endif