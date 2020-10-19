#ifndef _AIPLAYER_H
#define _AIPLAYER_H

#include "constants.h"
#include "board.h"
#include "stone.h"

int AIPlay(Board& Board, int AIcolor);
list<Stone> GetFreeStones(int color, Board Board);
Stone SelectMinmaxStone(Board Board, int color, list<Point>& chainmoves, int& Nb, int depth, list<int>& Action, int alpha, int beta);
void ClearPointerlist(list<list<Point>*>& list);
void ClearPointerlist(list<list<int>*>& pointerList);


#endif 