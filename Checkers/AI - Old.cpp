#pragma once
#include "stdafx.h"
#include "Board.h"
#include "Player.h"
#include "Utils.cpp"
#include "MoveGroup.cpp"
#include "PositionGroup.cpp"
#include <vector>

using namespace std;

/**
	The AI uses a basic hillclimbing algorithm to try and find the most number
	of jumps possible in a single turn. It calculates a score factoring in
	number of pieces taken, how far up the board it has moved, and how
	"in danger" it is, ie number of pieces surrounding it that can take
	it.

	The AI uses a weighted move tree, consisting of 5 possible move and a root position
	each move node is made up of its own score, plus the scores of its children
	nodes. The root node has no score of its own, and is simply an aggregate of 
	all the other scores. The path is then followed by the AI so it can achive
	the most moves in a single turn. One of these trees are constructed for each
	piece in play.

	   (80 + 20 + 10)
	     /     |  \
	 (60 + 20) 20 10
	    /  \
	   40  20
	A example node tree

**/
class AI{
	Board * b;
	MoveGroup move;

	//Weightings
	float score_weight;
	float up_weight;
	float danger_weight;

	vector<Piece *> playerPieces;

public:
	AI(Board * b, Player * p){
		score_weight = 5;
		up_weight = 1;
		danger_weight = -2;

		this->b = b;

		int top_current = -1;

		for(int i = 0; i < b->pieces.size(); i++){
			if(b->pieces[i].play == p){
				//playerPieces.push_back(&(b->pieces[i]));
			}
		}

		//Loop through every piece in play and find best move
		for(int i = 0; i < playerPieces.size(); i++){
			Piece * sel = playerPieces[i];
			MoveGroup mGroup = MoveGroup();
			PositionGroup positions = PositionGroup();

			check_move(sel, (*sel).x, (*sel).y, &mGroup, nullptr, positions);

			if(mGroup.total_score > top_current){
				top_current = mGroup.total_score;
				move = mGroup;
			}
		}

		//Run moves
		Move * next = &(move.all_moves[0]);
		bool cont = true;
		while(cont){
			b->moveTo(move.piece, (*next).x, (*next).y);

			int index = -1;
			float score = -1;

			for(int i = 0; i < (*next).next.size(); i++){
				float n_score = (*(*next).next[i]).score;
				if(n_score > score){
					score = n_score;
					index = i;
				}
			}

			next = (*next).next[index];
		}
	}

	void check_move(Piece * sel, int xs, int ys, MoveGroup * group, Move * last, PositionGroup positions){
		//Check every possible moving position
		//Jumps
		for(int x = xs-2; x < xs+2; x+=4){
			for(int y = ys-2; y < ys+2; y+=4){
				int inner_x = Utils::get_inner(x, xs), inner_y = Utils::get_inner(y, ys);
				if(b->pieceAtPosition(x, y) == nullptr && b->pieceAtPosition(inner_x, inner_y) != nullptr && positions.findPosition(inner_x, inner_y) < 0){
					//Check next move
					//Danger is not included because the piece will have moved
					float score = score_weight + ((y > 0) ? up_weight : 0);

					//Create the move it will do
					Move m = Move(x, y, last, score);
					//Add the move to the pool
					(*group).all_moves.push_back(m);
					//Link the move to the tree
					(*last).next.push_back(&(*group).all_moves[(*group).all_moves.size() - 1]);

					//Add the jump position to the list of position so the AI does not jump over the same checker twice
					positions.positions.push_back(Position(x, y));

					//Add score to all lower total scores
					Move * lm = &m;
					while((lm = m.last) != nullptr){
						(*lm).score += score;
					}

					//Add to group total score
					(*group).total_score += score;

					//Check next move, since it took a piece
					check_move(sel, x, y, group, &m, positions);
				}
			}
		}
		//Moves
		for(int x = xs-1; x < xs+1; x+=2){
			for(int y = ys-1; y < ys+1; y+=2){
				int danger_count = 0;
				//Check around
				for(int xa = x-1; xa < x+1; xa+=2){
					for(int ya = y-1; ya < y+1; ya+=2){
						if(b->pieceAtPosition(xa, ya) != nullptr){
							danger_count++;
						}
					}
				}

				float score = ((y > 0) ? up_weight : 0) + (danger_count * danger_weight);

				Move m = Move(x, y, last, score);
				(*group).all_moves.push_back(m);
				(*last).next.push_back(&(*(*group).all_moves.end()));

				Move * lm = &m;
				while((lm = m.last) != nullptr){
					(*lm).score += score;
				}

				(*group).total_score += score;
			}
		}
	}
};