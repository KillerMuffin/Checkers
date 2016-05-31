#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include "Move.cpp"
#include "Piece.h"

using namespace std;

class MoveGroup{
public:
	vector<Move> move_branch;
	vector<Move> move_leaf;
	float total_score;
	Piece * piece;

	MoveGroup(){
		total_score = 0;
		piece = nullptr;
	}
};