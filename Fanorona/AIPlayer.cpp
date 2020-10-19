#include <list>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include "AIPlayer.h"
#include "stone.h"
#include "board.h"
#include "constants.h"

int AIPlay(Board& Board, int AIcolor)
{
	
	int Nb;
	list<Point> chainmoves;
	list<int> Action;
	Stone SelectedStone = SelectMinmaxStone(Board, AIcolor, chainmoves, Nb, 0, Action, -1000, 1000);

	if (SelectedStone.x >= 0 && SelectedStone.y >= 0)
	{	
		while (chainmoves.empty() == false)
		{
			CaptureStones(SelectedStone, Board, chainmoves.back().x, chainmoves.back().y, Action.back(), true);
			chainmoves.pop_back();
			Action.pop_back();

			Show(Board);
		}
	}
	UnselectALL(Board);
	Show(Board);
	return 0;
}


list<Stone> GetFreeStones(int color, Board Board)
{
	int x, y;
	Stone TempStone;
	list<Stone> FreeStones;

	UnselectALL(Board);
	for (x = 0; x < Board.Columns; x++)
	{
		for (y = 0; y < Board.Rows; y++)
		{
			if (StoneExists(Board, x, y))
			{
				if (Board.Matrix[y][x] == color)
				{
					TempStone = NewStone(x, y, color);
					if (CanMove(TempStone, Board))
					{
						if (CanBeCaptured(TempStone, Board))
							FreeStones.push_back(TempStone);
						else
							FreeStones.push_front(TempStone);
					}
				}
			}
		}
	}
	return FreeStones;
}


Stone SelectMinmaxStone(Board Board_, int color, list<Point>& chainmoves, int& Nb, int depth, list<int>& Action, int alpha, int beta) 
{
	int limit = 3;
	
	list<Stone> FreeStones;
	FreeStones = GetFreeStones(color, Board_);
	if (FreeStones.empty() == true) {
		Nb = 0;
		return NewStone(-1, -1, -1);
	}

	int current_capture_num = 0;
	Stone TempStone = NewStone(-1, -1, -1);
	Stone SelectedStone = TempStone;
	Stone SelectedPaikaStone = TempStone;
	Board TempBoard;
	CreateBoard(TempBoard);
	InitializeBoard(TempBoard);

	list<Point> tempMoves;
	list<Point> laterMoves;
	list<int> tempActions;
	list<int> laterActions;

	list<list<Point>*> list_optimalMoves;
	list<list<Point>*> list_optimalPaikaMoves;
	list<list<int>*> list_optimalActions;
	list<list<int>*> list_optimalPaikaActions;

	int optimal_capture_num;
	int optimal_paika_capture_num;
	bool need_paika_move = true;


	if (depth % 2 == 0) {//MAX
		optimal_capture_num = -1000;
		optimal_paika_capture_num = -1000;
	}
	else {//MIN
		optimal_capture_num = 1000;
		optimal_paika_capture_num = 1000;
	}

	list<Stone>::iterator it;
	for (it = FreeStones.begin(); it != FreeStones.end(); it++)
	{
		TempStone = *it;
		Copy(Board_, TempBoard);
		tempMoves.clear();
		tempActions.clear();
		current_capture_num = CanMakeCapturingChainMove(TempStone, TempBoard, tempMoves, tempActions, 0);
		if (current_capture_num != 0) {
			need_paika_move = false;
		}
		UnselectALL(TempBoard);

		int later_capture_num = 0;
		int total_capture_num = 0;
		if (depth < limit - 1) {
			if (color == WHITE) {
				SelectMinmaxStone(TempBoard, BLACK, laterMoves, later_capture_num, depth + 1, laterActions, alpha, beta);
			}
			else if (color == BLACK) {
				SelectMinmaxStone(TempBoard, WHITE, laterMoves, later_capture_num, depth + 1, laterActions, alpha, beta);
			}

		}
		else
			later_capture_num = 0;
		if (depth % 2 == 0)
			total_capture_num = current_capture_num + later_capture_num;
		else
			total_capture_num = -current_capture_num + later_capture_num;

		UnselectALL(TempBoard);
		if (depth % 2 == 0) {
			if (current_capture_num != 0 && total_capture_num >= optimal_capture_num) {//non-paika move
				if (total_capture_num > optimal_capture_num) {
					ClearPointerlist(list_optimalMoves);
					ClearPointerlist(list_optimalActions);
				}
				optimal_capture_num = total_capture_num;

				list<Point>* temp = new list<Point>;
				copy(tempMoves.begin(), tempMoves.end(), back_inserter(*temp));
				list_optimalMoves.push_back(temp);

				list<int>* temp_intList = new list<int>;
				copy(tempActions.begin(), tempActions.end(), back_inserter(*temp_intList));
				list_optimalActions.push_back(temp_intList);

				SelectedStone = NewStone(TempStone.x, TempStone.y, TempStone.color);
				alpha = max(alpha, optimal_capture_num);
			
				if (beta <= alpha)
				{
					break;
				}
			}
			else if (current_capture_num == 0 && total_capture_num >= optimal_paika_capture_num) {//paika move
				if (total_capture_num > optimal_paika_capture_num) {
					ClearPointerlist(list_optimalPaikaMoves);
					ClearPointerlist(list_optimalPaikaActions);
				}
				optimal_paika_capture_num = total_capture_num;
				list<Point>* temp = new list<Point>;
				copy(tempMoves.begin(), tempMoves.end(), back_inserter(*temp));
				list_optimalPaikaMoves.push_back(temp);

				list<int>* temp_intList = new list<int>;
				copy(tempActions.begin(), tempActions.end(), back_inserter(*temp_intList));
				list_optimalPaikaActions.push_back(temp_intList);

				SelectedPaikaStone = NewStone(TempStone.x, TempStone.y, TempStone.color);

				alpha = max(alpha, optimal_paika_capture_num);
				
				if (beta <= alpha)
				{
					break;
				}
			}
		}
		else {//MIN
			if (current_capture_num != 0 && total_capture_num <= optimal_capture_num)//non-paika move
			{
				if (total_capture_num < optimal_capture_num) {
					ClearPointerlist(list_optimalMoves);
					ClearPointerlist(list_optimalActions);
				}

				optimal_capture_num = total_capture_num;

				list<Point>* temp = new list<Point>;
				copy(tempMoves.begin(), tempMoves.end(), back_inserter(*temp));
				list_optimalMoves.push_back(temp);

				list<int>* temp_intList = new list<int>;
				copy(tempActions.begin(), tempActions.end(), back_inserter(*temp_intList));
				list_optimalActions.push_back(temp_intList);

				SelectedStone = NewStone(TempStone.x, TempStone.y, TempStone.color);

				beta = min(beta, optimal_capture_num);
				
				if (beta <= alpha)
				{
					break;
				}
			}
			else if (current_capture_num == 0 && total_capture_num <= optimal_paika_capture_num) {//paika move
				if (total_capture_num < optimal_paika_capture_num) {
					ClearPointerlist(list_optimalPaikaMoves);
					ClearPointerlist(list_optimalPaikaActions);
				}
				optimal_paika_capture_num = total_capture_num;

				list<Point>* temp = new list<Point>;
				copy(tempMoves.begin(), tempMoves.end(), back_inserter(*temp));
				list_optimalPaikaMoves.push_back(temp);

				list<int>* temp_intList = new list<int>;
				copy(tempActions.begin(), tempActions.end(), back_inserter(*temp_intList));
				list_optimalPaikaActions.push_back(temp_intList);

				SelectedPaikaStone = NewStone(TempStone.x, TempStone.y, TempStone.color);
				beta = min(beta, optimal_paika_capture_num);
				
				if (beta <= alpha)
				{
					break;
				}
			}
		}
	}

	srand((unsigned int)time(NULL));

	if (!need_paika_move) {//non-paika move

		int num = rand();
		if (list_optimalMoves.size() > 0) {
			num = num % list_optimalMoves.size();

			list<list<Point>*>::iterator iter;
			list<list<int>*>::iterator iter_intList;
			iter_intList = list_optimalActions.begin();

			int i = 0;
			for (iter = list_optimalMoves.begin(); i < num; iter++) {
				i++;
				iter_intList++;
			}
			list<Point>* temp = *iter; 
			list<int>* temp_intList = *iter_intList;
			copy((*temp).begin(), (*temp).end(), back_inserter(chainmoves));
			copy((*temp_intList).begin(), (*temp_intList).end(), back_inserter(Action));
			
		}

		ClearPointerlist(list_optimalMoves);
		ClearPointerlist(list_optimalActions);
		ClearPointerlist(list_optimalPaikaMoves);
		ClearPointerlist(list_optimalPaikaActions);

		Nb = optimal_capture_num;
		return SelectedStone;
		
	}
	else {//paika move
		int num = rand();
		if (list_optimalPaikaMoves.size() > 0) {
			num = num % list_optimalPaikaMoves.size();

			list<list<Point>*>::iterator iter;
			list<list<int>*>::iterator iter_intList;
			iter_intList = list_optimalPaikaActions.begin();
			int i = 0;
			for (iter = list_optimalPaikaMoves.begin(); i < num; iter++) {
				i++;
				iter_intList++;
			}
			list<Point>* temp = *iter;
			list<int>* temp_intList = *iter_intList;
			copy((*temp).begin(), (*temp).end(), back_inserter(chainmoves));
			copy((*temp_intList).begin(), (*temp_intList).end(), back_inserter(Action));
		}

		ClearPointerlist(list_optimalMoves);
		ClearPointerlist(list_optimalActions);
		ClearPointerlist(list_optimalPaikaMoves);
		ClearPointerlist(list_optimalPaikaActions);

		Nb = optimal_paika_capture_num;
		return SelectedPaikaStone;
	}
}

void ClearPointerlist(list<list<Point>*>& pointerList) {
	for(list<Point>* pointer : pointerList) {

		delete pointer;
	}

	pointerList.clear();
}

void ClearPointerlist(list<list<int>*>& pointerList) {
	for (list<int>* pointer : pointerList) {
		delete pointer;
	}

	pointerList.clear();
}