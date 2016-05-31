#pragma once
#include "stdafx.h"
#include "Piece.h"
#include <vector>

using namespace std;

class Move{
public:
	int x, y;
	vector<Move *> next;
	int last;
	float score;

	Move(){
		x = -1;
		y = -1;
		last = -1;
		score = 0;
	}
	Move(int x, int y, int last, float score){
		this->x = x;
		this->y = y;
		this->last = last;
		this->score = score;
	}
};