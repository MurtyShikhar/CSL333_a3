#include <vector>
#include <iostream>
#include <Game.h>


using namespace std;


// minimax
template <typename EVAL>
class AIEngine{

private:
	EVAL eval;
	// int alpha;
	// int beta	

public:
	AIEngine(EVAL eval): eval(eval){ 
		/*alpha = -(1<<31);
		beta = (1 << 31);*/
	}

	move minimax(GameState& curr,int depth)
	{
		move curr = make_pair('X', make_pair(1,2));
		pair<int, move> m = make_pair(-(1<<31), curr);

		set<move> validMoves = gs.validMoves();
		typedef set<move>::iterator it;

		for (it = moves.begin(); it != moves.end(); it++)
		{
			GameState nxt = gs.action(*it);
			m = max(make_pair(Min(nxt, depth), *it), m);
		}

		return m.first;
	}


	int Max(GameState& gs,int depth)
	{
		if (gs.terminal() || depth == 0) return eval(gs);
		set<move> moves = gs.validMoves();
		typedef set<move>::iterator it;
		GameState nxt;
		int best = -(1<< 31);
		for (it = moves.begin(); it != moves.end(); it++)
		{
			nxt = gs.action(*it);
			best = max(best, Min(nxt, depth-1));
			// if (best > beta)
			// 	return best;
			// alpha = max(alpha, best);

		}
		return best;


	}


	int Min(GameState& gs, int depth)
	{
		if (gs.terminal() || depth == 0) return eval(gs);
		set<move> moves = gs.validMoves();
		typedef set<move>::iterator it;
		GameState nxt;
		int best = (1<< 31);
		for (it = moves.begin(); it != moves.end(); it++)
		{
			nxt = gs.action(*it);
			best = min(best, Max(nxt, depth -1));
			// if (best < alpha)
			// 	return best;
			// beta = min(best, alpha);

		}
		return best;
	}

}