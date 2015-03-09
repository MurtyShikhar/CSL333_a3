#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <cassert>
#include <string>
#include <utility>
#include <cmath>

using namespace std;

class Square{
public:

	int row, col;
	Square(int a,int b):row(a), col(b){}
    // a1, a2,a3,a4,a5,a6,a7,a8,a9 is the first column and so on
	Square(string s){
		if (s.length() > 1){
			row = s[1] - '1';
			col = s[0] - 'a';
		}
	}


	// copy constructor

	Square(const Square& other)
	{
		row = other.row, col = other.col;

	}
	// default constructor does nothing. 
	Square(){}
	pair<int,int> jumpPossible(Square& oth){
		int a = oth.row - row;
		int b = (oth.col ==  col);
		int c =  oth.col - col;
		int d = (oth.row == row);
		if (b && a == 1)
			return make_pair(1, 1);
		if (b && a == -1)
			return make_pair(1,-1);
		if (d && c == 1)
			return make_pair(-1,1);
		if (d && c == -1)
			return make_pair(-1,-1);

		return make_pair(0,0);

	}
	vector<Square> neighbors(int r) const{
		vector<Square> neighbors;
		for (int d = -r; d < r+1; d++){
			if (d != 0)
				if (row+d >= 0 && row+d < 9)
					neighbors.push_back(Square(row+d, col));
				if (col+ d >= 0 && col + d< 9)
					neighbors.push_back(Square(row, col+d));
		}

		return neighbors;
	}

	// true only if s lies in the same row or same column as this square
	bool cardinal(const Square& s) const 
	{
		return (row - s.row != 0) ^ (col - s.col != 0);
	}


	bool operator==(const Square& s) const{
		return (s.row == row) && (col == s.col);
	}

	bool operator!=(const Square& s) const {
		return !(*this == s);
	}

	bool operator<(const Square& s) const{
		return (row == s.row ? col < s.col : row < s.row);
	}

};

struct SquareHasher{
	size_t operator()(const Square& k) const{
		return (hash<int>()(k.row) ^ (hash<int>()(k.col) << 1));
	}
};
