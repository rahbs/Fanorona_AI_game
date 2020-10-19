#ifndef _STONE_H
#define _STONE_H

#include <list>
#include "constants.h"
#include "board.h"

using namespace std;

struct Stone
{
	int x;
	int y;
	int color;
	int History[ROWS][COLUMNS];
	Point LastPosition;
	int CapturedStones;
	int LastCapturedStones;
};

//Creates a New Stone
Stone NewStone(int x, int y, int color);

//Returns true if the move to NewX, NewY by the Stone is Legal else returns false
bool LegalMove(Stone Stone, Board Board, int NewX, int NewY);

//Returns the maximum number of stones that can be captured by any move from current location
//If No Capturing move can be made 0 is returned
int CanMakeCapturingMove(Stone Stone, Board Board, int& X, int& Y, int& Action);

//Returns the maximum number of stones that can be captured by any move from current location
//If No Capturing move can be made 0 is returned
int CanMakeCapturingChainMove(Stone Stone, Board& Board, list<Point>& chainmoves, list<int>& Action, int depth);

//Captures Stones on the Board by making a move to NewX, NewY by the action specified by Action which can be APPROACH or WITHDRAW
//The Move should be Legal
//The Stone and Board is only affected if the real variable is set to true
//Return Value: Number of Stones Captured
int CaptureStones(Stone& Stone, Board& Board, int NewX, int NewY, int Action, bool real);

//Returns the list of Stones that can be captured by Approaching the point NewX,NewY
list<Point> Approach(Stone Stone, Board Board, int NewX, int NewY);

//Returns the list of Stones that can be captured by Withdrawing to the point NewX,NewY
list<Point> Withdraw(Stone Stone, Board Board, int NewX, int NewY);

//Returns true if the stone can Make Legal moves from its current postion
bool CanMove(Stone Stone, Board Board, int* x = NULL, int* y = NULL);

//Makes a move to the point NewX, NewY
//Move has to be legal
//The Stone and Board are only affected if the real variable is set to true
//Returns the Action Taken
//If the Return Value is MUSTCHOOSE(which will be less than 0) the player must decide between APPROACH or WITHDRAW
int MakeMove(Stone& Stone, Board& Board, int NewX, int NewY, bool real, int& NB);

//Selects the Stone on the Board and Unselects others Selected stones if any
void Select(Stone Stone, Board& Board);

//Returns true if the stone is selected on the board
bool IsSelected(Stone Stone, Board Board);

//Unselects the Stone on the Board
void UnSelect(Stone Stone, Board& Board);

//Returns true if the Stone can be captured by the opponent given the current board status
bool CanBeCaptured(Stone Stone, Board Board);

//Returns true if the Stone is on a Strong Postion else false
bool IsOnStrongPosition(Stone Stone);

#endif