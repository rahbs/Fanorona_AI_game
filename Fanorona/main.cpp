#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include "constants.h"
#include "board.h"
#include "stone.h"
#include "AIPlayer.h"

int main()
{
	Board Board;
	Point SelectedLocation = NewPoint(-1, -1);
	Stone SelectedStone;
	
	int AIcolor;
	int HumanColor;
	int Nb;
	int X;
	int Y;
	int Action;
	int Status;
	int Winner;
	int mode;
	bool isMustChoice = false;
	bool isRunning = true;
	bool InvertColor = false;
	int Selected = WHITE;
	int HumanProgress = 0;
	int HumanMove = 0;


	printf("Panorona Game\n");
	printf("please select mode: \n");
	printf("1. AI vs AI\n");
	printf("2. AI vs human\n");
	scanf("%d", &mode);
	Sleep(SLEEP_MS);
	system("clear");

	if (mode == AI_MODE) {

		Selected = WHITE;
		CreateBoard(Board);
		InitializeBoard(Board);
		Show(Board);

		while (isRunning) {
			printf("AI_1: White(O) \nAI_2: Black(#)\n");

			if (Selected == WHITE)
				printf("WHITE turn");
			else
				printf("BLACK turn");

			AIPlay(Board, Selected);
			printf("AI_1: White(O) \nAI_2: Black(#)\n");
			Show(Board);


			UnselectALL(Board);
			if (Selected == WHITE)
				Selected = BLACK;
			else
				Selected = WHITE;

			SelectedStone = NewStone(-1, -1, -1);

			Winner = CheckWinner(Board);
			if (Winner != EMPTY)
			{
				Status = PAUSE;
				if (Winner == BLACK)
					printf("\nBLACK won the game\n");
				else
					printf("\nWHITE won the game\n");
				isRunning = false;
			}
		}

	}

	else if (mode == HUMAN_MODE) {
		HumanColor = WHITE;
		AIcolor = BLACK;
		HumanProgress = 0;
		Selected = WHITE;
		HumanMove = 0;
		Status = PLAY;

		CreateBoard(Board);
		InitializeBoard(Board);

		Show(Board);
		printf("Human Color: White(O) \nAIColor: Black(#)\n");
		printf("Human turn\n\n");
		printf("\n");
		while (isRunning)
		{
			SelectedLocation = NewPoint(-1, -1);

			int selected_x;
			int selected_y;

			if (Status == PLAY)
			{
				// AI turn
				if (AIcolor == Selected)
				{
					AIPlay(Board, AIcolor);

					InvertColor = true;
				}
				// Human turn
				else if (HumanColor == Selected)
				{
					if (isMustChoice == true)
					{
						gotoxy(16, 0);
						printf("select stone to take:");
						scanf("%d %d", &selected_x, &selected_y);
						SelectedLocation = NewPoint(selected_x, selected_y);
						isMustChoice = false;
					}
					switch (HumanProgress)
					{
					case 0:
					    gotoxy(16, 0);
						printf("select stone: ");
						scanf("%d %d", &selected_x, &selected_y);
						SelectedLocation = NewPoint(selected_x, selected_y);

						if (SelectedLocation.x >= 0 && SelectedLocation.y >= 0)
						{
							if (Board.Matrix[SelectedLocation.y][SelectedLocation.x] == HumanColor)
							{
								SelectedStone = NewStone(SelectedLocation.x, SelectedLocation.y, HumanColor);
								Select(SelectedStone, Board);
								HumanProgress = 1;
							}
							else
								printf("please choose white stone. \n");
						}
						else
							printf("please choose valid stone. \n");
						break;
					case 1:
						if (CanMakeCapturingMove(SelectedStone, Board, X, Y, Action) == 0
							|| CanMove(SelectedStone, Board) == false)
						{
							if (HumanMove != 0)
							{
								InvertColor = true;
								break;
							}
						}
						gotoxy(16, 0);
						printf("select location to move: ");
						scanf("%d %d", &selected_x, &selected_y);

						SelectedLocation = NewPoint(selected_x, selected_y);

						if (Board.Matrix[SelectedLocation.y][SelectedLocation.x] == HumanColor && HumanMove == 0)
						{
							SelectedStone = NewStone(SelectedLocation.x, SelectedLocation.y, HumanColor);
							Select(SelectedStone, Board);
							break;
						}
						if (SelectedLocation.x >= 0 && SelectedLocation.y >= 0)
						{
							if (Board.Matrix[SelectedLocation.y][SelectedLocation.x] == EMPTY)
							{
								if (CanMove(SelectedStone, Board) && LegalMove(SelectedStone, Board, SelectedLocation.x, SelectedLocation.y))
								{
									HumanMove = 1;

									if (MakeMove(SelectedStone, Board, SelectedLocation.x, SelectedLocation.y, true, Nb) == MUSTCHOOSE)
									{
										isMustChoice = true;
										X = SelectedLocation.x;
										Y = SelectedLocation.y;
										HumanProgress = 2;
										Nb = -1;
									}
									if (Nb == 0)
									{
										InvertColor = true;
									}
								}
							}
						}
						break;
					case 2:
						if (SelectedLocation.x >= 0 && SelectedLocation.y >= 0)
						{
							Nb = 0;
							Board.Matrix[SelectedStone.y][SelectedStone.x] = EMPTY;

							if (HumanColor == WHITE)
							{
								Board.Matrix[Y][X] = SELECTED_WHITE;
								if (Board.Matrix[SelectedLocation.y][SelectedLocation.x] == DELETE_APPROACHED_BLACK)
								{
									for (int i = 0; i < Board.Rows; i++)
									{
										for (int j = 0; j < Board.Columns; j++)
										{
											if (Board.Matrix[i][j] == DELETE_APPROACHED_BLACK)
											{
												Nb++;
												Board.Matrix[i][j] = EMPTY;
											}
											if (Board.Matrix[i][j] == DELETE_WITHDRAWED_BLACK)
												Board.Matrix[i][j] = BLACK;
										}
									}
								}
								else if (Board.Matrix[SelectedLocation.y][SelectedLocation.x] == DELETE_WITHDRAWED_BLACK)
								{
									for (int i = 0; i < Board.Rows; i++)
									{
										for (int j = 0; j < Board.Columns; j++)
										{
											if (Board.Matrix[i][j] == DELETE_APPROACHED_BLACK)
												Board.Matrix[i][j] = BLACK;
											if (Board.Matrix[i][j] == DELETE_WITHDRAWED_BLACK)
											{
												Nb++;
												Board.Matrix[i][j] = EMPTY;
											}
										}
									}
								}

							}
							else
							{
								Board.Matrix[Y][X] = SELECTED_BLACK;
								if (Board.Matrix[SelectedLocation.y][SelectedLocation.x] == DELETE_APPROACHED_WHITE)
								{
									for (int i = 0; i < Board.Rows; i++)
									{
										for (int j = 0; j < Board.Columns; j++)
										{
											if (Board.Matrix[i][j] == DELETE_APPROACHED_WHITE)
											{
												Nb++;
												Board.Matrix[i][j] = EMPTY;
											}
											if (Board.Matrix[i][j] == DELETE_WITHDRAWED_WHITE)
												Board.Matrix[i][j] = WHITE;
										}
									}
								}
								else if (Board.Matrix[SelectedLocation.y][SelectedLocation.x] == DELETE_WITHDRAWED_WHITE)
								{
									for (int i = 0; i < Board.Rows; i++)
									{
										for (int j = 0; j < Board.Columns; j++)
										{
											if (Board.Matrix[i][j] == DELETE_APPROACHED_WHITE)
												Board.Matrix[i][j] = WHITE;
											if (Board.Matrix[i][j] == DELETE_WITHDRAWED_WHITE)
											{
												Nb++;
												Board.Matrix[i][j] = EMPTY;
											}
										}
									}
								}
							}
							SelectedStone.LastPosition.x = SelectedStone.x;
							SelectedStone.LastPosition.y = SelectedStone.y;
							SelectedStone.x = X;
							SelectedStone.y = Y;
							SelectedStone.LastCapturedStones = Nb;
							SelectedStone.CapturedStones += SelectedStone.LastCapturedStones;
							SelectedStone.History[Y][X] = 1;
							if (Nb > 0)
								HumanProgress = 1;
						}
						break;
					}
				}

				Show(Board);
				printf("Human Color: White(O) \nAIColor: Black(#)\n");
				if (InvertColor)
				{
					InvertColor = false;
					HumanMove = 0;
					HumanProgress = 0;
					UnselectALL(Board);
					if (Selected == WHITE)
						Selected = BLACK;
					else
						Selected = WHITE;
					if (Selected == HumanColor)
					{
						gotoxy(0, 14);
						printf("Human turn\n");
					}
					else
					{
						gotoxy(0, 14);
						printf("AI turn\n");
						gotoxy(0, 16);
						SelectedStone = NewStone(-1, -1, -1);
					}
				}
				Winner = CheckWinner(Board);
				if (Winner != EMPTY)
				{
					Status = PAUSE;
					if (Winner == BLACK)
						printf("\nBLACK won the game\n");
					else
						printf("\nWHITE won the game\n");
				}
			}
		}
	}
	else {
		printf("please select valid number \n");
	}
	return 0;
}