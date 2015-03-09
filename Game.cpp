#include "Game.h"


stack<Square> GameState::shortestPathtorow(Square src, int row){
	stack<Square> path;
	queue<Square> q;
	unordered_map<Square, Square, SquareHasher> parentNode;
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
			if (parentNode.find(*i) == parentNode.end())
			{
				parentNode[*i] = t;
				q.push(*i); 
			}
		}

	}

	return path;
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