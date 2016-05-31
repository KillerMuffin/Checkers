// Checkers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Board.h"
#include "AI.cpp"
#include <string>
#include "Position.cpp"

class Game{
public:
	Board b;
	Player u;
	Player aip;
	
	Game(){

	}

	void play(){
		u = Player();
		u.ident = 'r';
		u.dir = 1;

		aip = Player();
		aip.ident = 'b';
		u.dir = -1;

		b = Board();

		for(int y = 0; y < 2; y++){
			for(int x = 0; x < Utils::size; x++){
				Piece p = Piece(x,y,&aip);
				b.pieces.push_back(p);
			}
		}

		for(int y = Utils::size - 1; y > Utils::size - 3; y--){
			for(int x = 0; x < Utils::size; x++){
				Piece p = Piece(x,y,&u);
				b.pieces.push_back(p);
			}
		}

		bool playing = true;
		while(playing){
			b.print();

			string from;
			string to;
			Piece * piece = get_piece();
			
			if(piece == nullptr || (*piece).play != &u){
				Utils::error("Invalid selection");
				continue;
			}

			bool repeat = true;
			bool cont = false;

			while(repeat){
				cout << "To: ";
				cin >> to;

				Position to_p = parse_position(to);

				if(b.pieceAtPosition(to_p.x, to_p.y) != nullptr){
					Utils::error("Piece already at that position");
					b.print();
					repeat = true;
					continue;
				}

				MoveLevel m = b.isLegal(piece->x, piece->y, to_p.x, to_p.y, piece);

				if(m == MoveLevel::ILLEGAL){
					Utils::error("Illegal move!");
					b.print();
					repeat = true;
					continue;
				}

				repeat = (m == MoveLevel::CONTINUE);

				b.moveTo(piece, to_p.x, to_p.y);

				if(repeat){
					Utils::cls();
					b.print();
				}
			}
			Utils::cls();
			
			cout << "The AI is thinking..." << endl;
			
			AI ai = AI(&b, &aip);

			Utils::clp();

			Utils::cls();
		}
	}

	void printIntro(){
		cout << "          )               )     (    (     \r\n   (   ( /(       (    ( /(     )\\ ) )\\ )  \r\n   )\\  )\\())(     )\\   )\\())(  (()/((()/(  \r\n (((_)((_)\\ )\\  (((_)|((_)\\ )\\  /(_))/(_)) \r\n )\\___ _((_|(_) )\\___|_ ((_|(_)(_)) (_))   \r\n((/ __| || | __((/ __| |/ /| __| _ \\/ __|  \r\n | (__| __ | _| | (__  ' < | _||   /\\__ \\  \r\n  \\___|_||_|___| \\___|_|\\_\\|___|_|_\\|___/  \r\n    " << endl;
		cout << "v1.0 Ben McLean" << endl;
	}

	Piece * get_piece(){
		string input;
		cout << "Select piece: ";
		cin >> input;

		Position p = parse_position(input);
		return b.pieceAtPosition(p.x, p.y);
	}

	Position parse_position(string s){
		if(Utils::isLetter(s[0])){
			int x = Utils::toLower(s[0]) - 97;
			int y = stoi(s.substr(1)) - 1;

			return Position(x,y);
		}else{
			int x = Utils::toLower(s[s.size() - 1]) - 97;
			int y = stoi(s.substr(0, s.size()-1)) - 1;

			return Position(x,y);
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Game game = Game();
	
	game.printIntro();

	Utils::clp();
	
	game.play();

	return 0;
}