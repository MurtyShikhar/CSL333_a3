#include "Player.h"

typedef double Feature;    //readability

class BasePlayer : public Player
{


private:

	int findGapsIncols()
	{
		int gaps = 0;
		for (int col = 0; col < 9; col++)
		{
			int gapsinCurrCol = 0;
			int row = 0;
			bool Walls = false;
			while (row < 9)
			{
				Square currSquare(row,col);
				bool curr = (gs.boardGraph[currSquare].find(Square(row,col+1)) != gs.boardGraph[currSquare].end() ); // gap found in this row,and this col
				if(!curr)
				{
					Walls = true;
				}
				if (curr) {if (row == 0) gapsinCurrCol++; row++; continue;}
				for (int j =row+1; j < 9; j++) // no gaps found till j-1
				{
					currSquare.row = j;
					bool check = (gs.boardGraph[currSquare].find(Square(row,col+1)) != gs.boardGraph[currSquare].end() );
					if (check != curr)
					{
						gapsinCurrCol++;
						row = j+1; 
						break;
					}
				}
			}
			if(Walls)
				gaps += gapsinCurrCol;
		}


		return gaps;
	}

	int findGapsInrows()
	{
		int gaps = 0;
		for (int row = 0; row < 9; row++)
		{
			int gapsinCurrRow = 0;
			int col = 0;

			while (col < 9)
			{
				Square currSquare(row,col);
				bool curr = (gs.boardGraph[currSquare].find(Square(row+1, col)) != gs.boardGraph[currSquare].end() );
				if (curr) {if (col == 0) gapsinCurrRow++; col++; continue;}
			
				for (int j = col+1; j < 9; j++)
				{
					currSquare.col = j;
					bool check = (gs.boardGraph[currSquare].find(Square(row+1, j)) != gs.boardGraph[currSquare].end() );
					if (check != curr)
					{	
					
						gapsinCurrRow++;
						col = j +1;
						break;
					}
				}

				

			}

			gaps+= gapsinCurrRow;
		}

		return gaps;
		
	}

	int gapCount(GameState& gs){

		int gaps = 0;
		gaps += findGapsInrow();
		gaps += findGapsIncols(); 
		return gaps;
	}


	double pathCount(GameState& gs)
	{
		int gaps = gapCount(gs);

	}


public:
	double operator()(GameState& gs, Square& myPlayer)
	{
		double eval;

		Feature wallsLeft, shortestDistToGoal;
		shortestDistToGoal = gs.shortestPathToWin() - gs.otherShortestPathToWin();
		wallsLeft = gs.currentPlayerWalls() - gs.otherPlayerWalls();
		if(myPlayer != gs.currentPlayer())
		{
			shortestDistToGoal = -shortestDistToGoal;
			eval += shortestDistToGoal;                    //this will be weighted
			wallsLeft = -wallsLeft;
			eval += shortestDistToGoal;                    //this will be weighted
		}
		return eval;
	}
};