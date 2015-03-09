#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <cassert>
#include <string>
#include <utility>
#include "Square.h"
#include "Wall.h"

using namespace std;


typedef pair<char, pair<int, int> >   move;

typedef unordered_map<Square ,set<Square> , SquareHasher> Graph;

class GameState{
private:
	static const int BOARD_SIZE = 9;
	int numWallsA, numWallsB; 
	Square player1, player2;
	int turn;
	Graph boardGraph;
	set<Wall> walls; // on the board

	void addEdge(Square a, Square b){
		boardGraph[a].insert(b);
		boardGraph[b].insert(a); // edges are undirected
	}

	void removeEdge(Square a, Square b);

	int currentPlayerWalls(){
		if (!currentPlayer()) return numWallsA;
		return numWallsB;
	}



	bool isValidWallPlacement(Wall wall);

public:
	GameState(): player1("e1"), player2("e9"), numWallsA(10), numWallsB(10), turn(0){
		for (int i = 0; i < BOARD_SIZE; i++){

			for (int j = 0; j < BOARD_SIZE; j++){
				set<Square> adj;
				for (int d = -1; d < 2; d++){
					if (d != 0) {
						if (i+d >= 0 && i+d < BOARD_SIZE)
							adj.insert(Square(i+d, j));
						if (j+d >= 0 && j+d < BOARD_SIZE)
							adj.insert(Square(i, j+d));
					}
				}
				boardGraph.insert(make_pair(Square(i,j), adj) );
			}
		}
	}
	// add squares a and b to each other's adjacency lists

	set<move> validMoves();
	GameState action(move);

	int currentPlayer(){ return turn; }  // 0 if player 1, 1 if player 2
	int otherPlayer() {return !turn;}
	int otherPlayerPosition() {return currentPlayer() == 0? player2: player1;}
	
	Square currentPlayerPosition(){ return currentPlayer() == 0 ? player1: player2; }
	
	bool terminal() { return player1.row == 8 || player2.row == 0; }  // test for terminal state

	
	bool hasPathtoGoal(){
		return (!shortestPathtorow(player1, 0).empty() && !shortestPathtorow(player2, 8).empty());
	}
	
	void placeWall(Wall wall);

	stack<Square> shortestPathtoWin(){
		if (!currentPlayer()) return shortestPathtorow(player1, 0);
		else return shortestPathtorow(player2, 8);
	}


	// need a way to handle jumps..
	// just a simple BFS
	stack<Square> shortestPathtorow(Square src, int row);

	void traverse(Square sq) {
		if (currentPlayer() == 0) player1 = sq;
		else player2 = sq;
	}


};