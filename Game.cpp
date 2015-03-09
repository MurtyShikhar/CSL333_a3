#include "Game.h"


stack<Square> GameState::shortestPathtorow(Square src, int row){
	stack<Square> path;
	queue<Square> q;
	unordered_map<Square, Square, SquareHasher> parentNode; // explored nodes
	parentNode[src] = Square(-1,-1);
	q.push(src);
	typedef set<Square>::iterator itr;

	while (!q.empty()) {
		Square t = q.front(); q.pop();
		if (t.row == row){
			while (t != src){
				path.push(t);
				t = parentNode[t];
			}
			return path;
		}
		set<Square> s = boardGraph[t];
		for (itr i = s.begin(); i != s.end(); i++){
			if (parentNode.find(*i) == parentNode.end()) // if not yet explored, explore
			{
				parentNode[*i] = t;
				q.push(*i); // push it in the queue once explored
			}
		}

	}

	return path;
}

GameState GameState::action(move m)
{
	int vertical = (m.first == 'V'); // vertical is 1, if vertical is 0, then prolly horizontal is 0 
	int horizontal = !(m.first == 'H'); // horizontal is 0, if 1 then wall is not horizontal
	
	GameState g = *this;
	if (!vertical && horizontal){
		int xCoord = m.second.first;
		int yCoord = m.second.second;
		g.traverse(Square(xCoord, yCoord));
		g.turn = !turn;
		pair<int,int> j = jumpPossible(player1, player2);
		if(j.first){
			if (j.first == 1){
				Square pos = g.otherPlayerPosition();
				int allow = pos.col + j.second <= 7 && pos.col + j.second >= 0;
				if (!allow) return g;
				g.addEdge(g.currentPlayerPosition(), Square(pos.row, pos.col + j.second) );
			}
			if (j.first == -1) {
				Square pos = g.otherPlayerPosition();
				int allow = pos.row + j.second <= 7 && pos.row + j.second >= 0;
				if (!allow) return g;
				g.addEdge(g.currentPlayerPosition(), Square(pos.row + j.second, pos.col));

			}
		}
		return g;
	}

	int xCoord = m.second.first;
	int yCoord = m.second.second;
	Wall w(vertical || horizontal, xCoord, yCoord);
	g.placeWall(w);
	return g;
}

set<move> GameState::validMoves()
{
	set<move> moves;
	set<Wall> walls;
	int wallsRemaining = (turn? numWallsA : numWallsB);
	if (currentPlayerPosition().row < 9)
		moves.insert(make_pair('T', make_pair(currentPlayerPosition().row+1,currentPlayerPosition().col) ));
	if (currentPlayerPosition().col < 9)
		moves.insert(make_pair('T', make_pair(currentPlayerPosition().row, currentPlayerPosition().col +1)));

	if (currentPlayerPosition().col > 0)
		moves.insert(make_pair('T', make_pair(currentPlayerPosition().row, currentPlayerPosition().col -1)));
	if (!wallsRemaining){ return moves;}
	for (int i = 0; i < wallsRemaining; wallsRemaining++)
	{
		for (int j = 0; j < 8; i++)
			for (int k =0; k < 8; k++){
				Wall w1(0, j,k); Wall w2(1, j,k); 
				if (walls.find(w1) == walls.end()) moves.insert(make_pair('H', make_pair(j,k) ) ); 
				if (walls.find(w2) == walls.end()) moves.insert(make_pair('V', make_pair(j,k) ) );
			}
	}

	return moves;


}

void GameState::placeWall(Wall wall)
{
	int xCoord =wall.coords.first;
	int yCoord = wall.coords.second;
	if (!isValidWallPlacement(wall)) return;
	if (!currentPlayer()) numWallsA--;
	else numWallsB--;
	walls.insert(wall);
	if (wall.orientation == VERTICAL)
	{
		removeEdge(Square(xCoord, yCoord), Square(xCoord +1, yCoord));
		removeEdge(Square(xCoord, yCoord+1), Square(xCoord+1, yCoord +1));
	}	

	else{
		removeEdge(Square(xCoord, yCoord), Square(xCoord, yCoord +1));
		removeEdge(Square(xCoord+1, yCoord), Square(xCoord+1, yCoord+1));
	}
}

// assume that the wall coordinates are correct i.e. between 0 and 7
bool GameState::isValidWallPlacement(Wall wall){
	int xCoord =wall.coords.first;
	int yCoord = wall.coords.second;
	if (currentPlayerWalls() == 0) return false;
	
	if (walls.find(wall) != walls.end()) // check if wall is already present
		return false;
	if (wall.orientation == VERTICAL)
	{
		Wall crossingNeigbor1(HORIZONTAL, xCoord, yCoord); // intersection with a horizontal wall
		Wall crossingNeigbor2(VERTICAL, xCoord,yCoord +1); // intersection with a vertical wall one row above
		Wall crossingNeigbor3(VERTICAL, xCoord, yCoord -1);	// intersection with a vertical wall one row below
		if (walls.find(crossingNeigbor1) != walls.end() || walls.find(crossingNeigbor2) != walls.end() || walls.find(crossingNeigbor3) != walls.end())
			return false;
		// if wall doesn't intersect, update the boardGraph as follows

		removeEdge(Square(xCoord, yCoord), Square(xCoord+1, yCoord)); 
		removeEdge(Square(xCoord, yCoord+1), Square(xCoord+1, yCoord +1));

		bool hasPath = hasPathtoGoal();

		// add the edges back
		addEdge(Square(xCoord, yCoord), Square(xCoord+1, yCoord));
		addEdge(Square(xCoord, yCoord+1), Square(xCoord+1, yCoord +1));
		if (!hasPath)
			return false;


	}

	if (wall.orientation == HORIZONTAL)
	{
		Wall crossingNeigbor1(VERTICAL, xCoord , yCoord);  // intersection with a vertical wall
		Wall crossingNeigbor2(HORIZONTAL, xCoord +1, yCoord);	// intersection with a horizontal wall one column to the right
		Wall crossingNeigbor3(HORIZONTAL, xCoord -1, yCoord); // intersection with a horizontal wall one column to the left
		if (walls.find(crossingNeigbor1) != walls.end() || walls.find(crossingNeigbor2) != walls.end() || walls.find(crossingNeigbor3) != walls.end())
			return false;
		// if wall doesn't intersect, update the boardGraph as follows

		removeEdge(Square(xCoord, yCoord), Square(xCoord, yCoord +1));
		removeEdge(Square(xCoord+1, yCoord), Square(xCoord+1, yCoord+1));

		bool hasPath = hasPathtoGoal();
		// add the edges back
		addEdge(Square(xCoord, yCoord), Square(xCoord, yCoord +1));
		addEdge(Square(xCoord+1, yCoord), Square(xCoord+1, yCoord+1));
		if (!hasPath)
			return false;
	}

	return true;

}