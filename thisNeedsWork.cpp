
class gameState{

private:
	Square playerA,playerB;
	int numWallsA, numWallsB;
	bool turn;
	
	bool isValidWallPlacement(Wall wall){
		if (currentPlayerWalls() == 0) return false;
		if (wallBlock(wall)) return false;
		placeWall(wall);
		UnionFind u(board, playerB, playerA);
		deleteWall(wall);
		if (!u.hasPathtoGoal()) return false;
		return true;
	}

	void deleteWall(Wall wall){
		int row = wall.coords.first;
		int column = wall.coords.second;
		
		//remove the walls via bit manipulation
		board &= ~(1 << row);
		board &= ~(1 << column);

		// restore wall count for the current player
		if (!turn) numWallsA++;
		numWallsB++;

	}	
	bool wallBlock(Wall wall){
		int row = wall.coords.first;
		int column = wall.coords.second;
		return (board & (1<< row) ) && (board & (1 << column));
	}


public:	
	long board; // 64 bit board representation

	// simple functions
	int currentPlayer(){return turn;}
	Square currentPlayerPosition(){if (!turn) return playerA; else return playerB;}
	Square otherPlayerPosition() {if (currentPlayer()) return playerB; else return playerA;}
	bool terminal(){return playerA.row == 8 || playerB.row == 0;}

	int currentPlayerWalls(){
		if (!currentPlayer()) return numWallsA;
		return numWallsB;
	}
	
	int otherPlayerWalls(){
		if (!currentPlayer()) return numWallsB;
		return numWallsA;
	}



	void traverse(Square sq) {
		if (currentPlayer() == 0) playerA = sq;
		else playerB = sq;
	}




	bool hasPathtoGoal(){
		UnionFind u(board, playerA, playerB);
		return u.hasPathtoGoal();
	}

	void placeWall(Wall wall){
		if (!isValidWallPlacement(wall)) return;
		int row = wall.coords.first, column = wall.coords.second;
		
		board |= 1 << row;
		board |= 1 << column;
		if (!turn) numWallsA--;
		numWallsB--;
	}


};




class UnionFind{
	long representation; // 0...7 rows, and 0...7 columns
	Square pA, pB;

	bool Connected(Square, Square);
	
	bool disconnect(Square, Square);
};



class BFS{


public:	
	long representation;
	Square posA, posB;

	BFS(long r, Square& a, Square& b): representation(r), posA(a), posB(b){}


	int shortestPathtorow(int row);
};