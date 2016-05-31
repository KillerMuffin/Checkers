#include "stdafx.h"
#include <string>
#include <iostream>

using namespace std;

class PrintBoard{
	int boardSize;
public:
	PrintBoard(int boardSize);
	PrintBoard();
	void printLine(int size);
	void printLine();
	
	void printCell(char character);
	void printCell();
};