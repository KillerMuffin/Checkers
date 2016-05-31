#pragma once
#include "stdafx.h"
#include <string>
#include <iostream>
#include "PrintBoard.h"
#include "Player.h"
#include "Utils.cpp"
#include "Piece.h"
#include <vector>
#include "MoveLevel.cpp"

using namespace std;

class Board{
public:
	PrintBoard prB;
	vector<Player> players;
	vector<Piece> pieces;

	Board();
	void print();
	Player * playerAtPosition(int x, int y);
	Piece * pieceAtPosition(int x, int y);

	void moveTo(Piece * piece, int x, int y);

	int calculateDistance(int x1, int y1, int x2, int y2);

	MoveLevel isLegal(int x, int y, int nx, int ny, Piece * p);
};