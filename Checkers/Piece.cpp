#include "stdafx.h"
#include "Piece.h"

Piece::Piece(int x, int y, Player * player){
	this->x = x;
	this->y = y;
	this->play = player;
	this->king = false;
}