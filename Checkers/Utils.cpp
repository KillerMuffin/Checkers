#pragma once
#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h> 

using namespace std;

class Utils{
public:
	const static int size = 12;
	const static int pieces = size * 2;

	static int get_inner(int c, int cs){
		return ((c-cs)/2)+cs;
	}

	static int dif(int a, int b){
		return abs(a - b);
	}

	static char toLower(char c){
		//If [A-Z]
		if(c >= 65 && c <= 90){
			return (char)(c + 32);
		}

		return c;
	}

	static char toUpper(char c){
		//If [A-Z]
		if(c >= 97 && c <= 122){
			return (char)(c - 32);
		}

		return c;
	}

	static bool isLetter(char c){
		//If [A-Z] or [a-z]
		return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
	}

	static void cls(){
		system("cls");
	}

	static void pau(){
		system("pause");
	}

	static void clp(){
		system("pause");
		system("cls");
	}

	static void error(string message){
		cls();
		cout << message << endl;
		clp();
	}

	static int random(int min, int max){
		srand(time(NULL));
		return rand()%(max-min + 1) + min;
	}
};