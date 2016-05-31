#pragma once
#include "stdafx.h"
#include "Player.h"
#include <string>

using namespace std;

class Piece{
public:
	Player * play;
	int x;
	int y;

	bool king;

	Piece(int x, int y, Player * player);
};