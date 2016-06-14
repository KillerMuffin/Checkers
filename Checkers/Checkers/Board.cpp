#include "stdafx.h"
#include "Board.h"
#include <cmath>
#include "CannotMoveException.cpp"

Board::Board(){
	prB = PrintBoard(Utils::size);
}

Player * Board::playerAtPosition(int x, int y){
	Piece * p = pieceAtPosition(x, y);
	return (p == nullptr) ? nullptr : (*p).play;
}

Piece * Board::pieceAtPosition(int x, int y){
	for(int i = 0; i < pieces.size(); i++){
		if(pieces[i].x == x && pieces[i].y == y){
			return &(pieces[i]);
		}
	}
	return nullptr;
}

void Board::moveTo(Piece * piece, int x, int y){
	if(this->calculateDistance((*piece).x, (*piece).y, x, y) > 1){
		int mx = Utils::get_inner(x, (*piece).x);
		int my = Utils::get_inner(y, (*piece).y);

		for(int i = 0; i < pieces.size(); i++){
			if(pieces[i].x == mx && pieces[i].y == my){
				pieces.erase(pieces.begin() + i);
			}
		}
	}

	(*piece).x = x;
	(*piece).y = y;
}

MoveLevel Board::isLegal(int x, int y, int nx, int ny, Piece * p){
	if(nx < 0 || nx >= Utils::size -1 || ny < 0 || ny >= Utils::size -1){
		return MoveLevel::ILLEGAL;
	}
	if(nx == x || ny == x){
		return MoveLevel::ILLEGAL;
	}

	int distance = this->calculateDistance(x, y, nx, ny);
	if(
		this->pieceAtPosition(nx, ny) != nullptr ||

		distance > 2 || distance < 1 ||

		(distance == 2 && 
		this->pieceAtPosition(
			Utils::get_inner(nx, x), 
			Utils::get_inner(ny, y)
		) == nullptr)){
		return MoveLevel::ILLEGAL;
	}

	/*if(!p->king && (ny - y) * p->play->dir < 0){
		return MoveLevel::ILLEGAL;
	}*/

	if(distance == 2){
		return MoveLevel::CONTINUE;
	}

	return MoveLevel::LEGAL;
}

int Board::calculateDistance(int x1, int y1, int x2, int y2){
	int x_dif = Utils::dif(x1, x2);
	int y_dif = Utils::dif(y1, y2);

	float hyp = sqrt(pow(x_dif, 2) + pow(y_dif, 2));

	return floor(hyp);
}

void Board::print(){
	for(int y = 0; y < Utils::size; y++){
		prB.printLine();
		for(int x = 0; x < Utils::size; x++){
			if(pieceAtPosition(x, y) != nullptr){
				Piece p = (*pieceAtPosition(x, y));
				prB.printCell((p.king) ? Utils::toUpper((*p.play).ident) : (*p.play).ident);
			}else{
				prB.printCell(' ');
			}
		}
		cout << " | " << (y + 1) << endl;
	}
	prB.printLine();

	for(int x = 0; x < Utils::size; x++){
		cout << "   " << (char)('A' + x);
	}
	cout << endl;
}