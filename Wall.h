#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <cassert>
#include <string>
#include <utility>

using namespace std;


#define VERTICAL 1
#define HORIZONTAL 0

struct Wall{
	int orientation;
	

	pair<int,int> coords;   // coordinates of the wall
	/*
		coords.first is the x coordinate of the wall, and coords.second is the y coordinate of the wall
		for a horizontal wall the x coordinate ranges from 0 to 7 and the y coordinate ranges from 0 to 7
		
		for a horizontal wall to intersect with a vertical wall, their x and y coordinates need to be the same
	*/
	Wall(int a, int b,int c) {
		coords = make_pair(b,c);
		orientation = a;
		
	}
	Wall(string a)
	{
		orientation = a[0] == 'V'? 1 : 0;
		int xCoord = a[1] - 'a';
		int yCoord = a[2] - '1';
		assert (xCoord >= 0 && xCoord <= 7 && yCoord >= 0 && yCoord <= 7); // sanity check
		coords = make_pair(xCoord, yCoord);
	}

	/* to support set insertion */
	bool operator<(const Wall& that) const
	{
		if (coords != that.coords)
			return coords < that.coords;
		return orientation < that.orientation;
	}

	bool operator==(const Wall& that) const
	{
		return coords == that.coords && orientation == that.orientation;
	}	
	
};