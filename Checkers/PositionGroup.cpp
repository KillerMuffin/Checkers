#pragma once
#include "stdafx.h"
#include "Position.cpp"
#include <vector>

using namespace std;

class PositionGroup{
public:
	vector<Position> positions;
	
	int findPosition(int x, int y){
		for(int i = 0; i < positions.size(); i++){
			if(positions.at(i).x == x && positions.at(i).y == y){
				return i;
			}
		}

		return -1;
	}
};