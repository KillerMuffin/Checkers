#include "stdafx.h"
#include "PrintBoard.h"
#include "Utils.cpp"

PrintBoard::PrintBoard(int boardSize){
	this->boardSize = boardSize;
}
PrintBoard::PrintBoard(){
	this->boardSize = Utils::size;
}

void PrintBoard::printLine(int size){ //3 characters per square
	string s = "";
	for(int i = 0; i < size; i++){
		s+='-';
	}
	
	cout << s << endl;
}

void PrintBoard::printLine(){
	this->printLine((this->boardSize * 4) + 2);
}

void PrintBoard::printCell(char character){
	cout << " | " << character;
}

void PrintBoard::printCell(){
	this->printCell(' ');
}