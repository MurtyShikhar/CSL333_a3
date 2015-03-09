#include "Player.h"

typedef double Feature;    //readability

class BasePlayer : public Player
{
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
	}
};