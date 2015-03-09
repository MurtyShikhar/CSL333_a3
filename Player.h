#include <iostream>
#include <vector>
#include <set>
#include "Game.h"

using namespace std;

class Player {
public:
	virtual double operator()(GameState& gs, Square& myPlayer) = 0;   //need to know if my player is player 1 or player 2
};