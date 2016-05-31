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

The AI uses a weighted move tree, consisting of "move nodes" that contain an x and y
position, its aggregate score, and the last node. This tree is generated using the 
recursive function "check_move", which follows all the possible paths calculating the
score (the score equals the calculated score plus the score of the nodes before it
and storing the tree. Each tree is stored in a "MoveGroup", which holds the tree
and the piece it relates too. The tree is stored as two vector arrays, "branches" and
"leaves". This allows the use a flat and efficient data structure, while still allowing
the tree to be searched by iterating through the leaves.

When the tree is finished being built the AI then iterates through the leaves to find the
highest scoring ones, putting them into a "selection pool". The selection pool is nessesary
so that the AI does not have a bias, otherwise the AI would have a preference for moving
backwards and to the left. A random leaf is selected from the pool, and the AI follows it
down, completing its move.

A example move tree (x, y (score)):
                    6,0 (origin)
                     |
  /---------/--------|--------\----------\
3,2 (7)   3,3 (8)   8,2 (7)   5,2 (7)   5,3 (8)
                     |
      /---------/----|----\-------\
    7,2 (7)   7,3(8)   9,2 (7)   9,3 (8)

The selection pool from this node tree:
(3,3), (7,3), (9,3), (5,3)
The path randomly selected from the pool:
(6,0) -> (8,2) -> (9,3)

**/
class AI{
	Board * b;
	//All pieces moves
	vector<MoveGroup> moves;

	//Selection pool
	vector<Position> mIndex; //index of highest movegroup (x), and leaf (y)

	//Weightings
	float score_weight;
	float up_weight;
	float danger_weight;

	//Store all the players selected pieces
	vector<Piece *> playerPieces;

public:
	AI(Board * b, Player * p){
		score_weight = 5;
		up_weight = 1;
		danger_weight = -2;

		this->b = b;

		//Get all player's pieces
		for(int i = 0; i < b->pieces.size(); i++){
			if(b->pieces[i].play == p){
				playerPieces.push_back(&(b->pieces[i]));
			}
		}

		//Generate move tree
		for(int i = 0; i < playerPieces.size(); i++){
			MoveGroup group = MoveGroup();
			group.piece = playerPieces[i];
			PositionGroup positionGroup = PositionGroup();

			//Recursive tree builder
			check_move(playerPieces[i], (*playerPieces[i]).x, (*playerPieces[i]).y, &group, -1, positionGroup);

			moves.push_back(group);
		}

		int score = -1;

		//Find best moves
		for(int i = 0; i < moves.size(); i++){
			vector<Move> move_tree = moves[i].move_leaf; 

			for(int j = 0; j < move_tree.size(); j++){
				if(move_tree[j].score == score){
					//Add move to the pool
					mIndex.push_back(Position(i, j));

				}else if(move_tree[j].score > score){
					//Empty the pool and add the new highest scoring
					score = move_tree[j].score;

					mIndex.empty();
					mIndex.push_back(Position(i,j));
				}
			}
		}

		cout << "Winning score: " << score << endl;
		//Select a random move from the pool
		int fi = Utils::random(0, mIndex.size() - 1);
		MoveGroup g = moves[mIndex[fi].x];
		Move * m = &(moves[mIndex[fi].x].move_leaf[mIndex[fi].y]);

		//Print debug tree
		printTree(&g);

		//Move piece to final position
		int ex = m->x;
		int ey = m->y;
		int sx = g.piece->x;
		int sy = g.piece->y;
		cout << "Final Move " << sx << ", " << sy << " to " << ex << ", " << ey << endl;
		g.piece->x = ex;
		g.piece->y = ey;

		//Go backwards to the start
		while(m->last >= 0){
			m = &g.move_branch[m->last];
			cout << "Move " << g.piece->x << ", " << g.piece->y << " to " << m->x << ", " << m->y << endl;
			b->moveTo(g.piece, m->x, m->y);
		}

		cout << "Start Move " << sx << ", " << sy << " to " << g.piece->x << ", " << g.piece->y << endl;
		b->moveTo(g.piece, sx, sy);

		//Move back to final position
		g.piece->x = ex;
		g.piece->y = ey;
	}

	//Recursive tree function
	/**
	 @param sel The piece to follow
	 @param xs The start x position
	 @param ys The start y position
	 @param last The index of the last inserted move
	 @param positions The positions the AI has already jumped over, used to prevent the AI
		from scoring on the same piece twice and getting stuck in an infinite loop
	**/
	void check_move(Piece * sel, int xs, int ys, MoveGroup * group, int last, PositionGroup positions){
		//Loop through jump positions
		for(int x = xs - 2; x <= xs + 2; x+=4){
			for(int y = ys - 2; y <= ys + 2; y+=4){

				Piece p = Piece(xs, ys, sel->play);
				//Check if legal
				MoveLevel m = b->isLegal(xs, ys, x, y, &p);

				if(m != MoveLevel::ILLEGAL){
					int ix = Utils::get_inner(x, xs);
					int iy = Utils::get_inner(y, ys);

					if(positions.findPosition(ix, iy) < 0){
						//No danger rating is calculated because the piece will be moved
						int score = score_weight + ((y - ys) * up_weight) + ((last >= 0) ? (*group).move_branch[last].score : 0);
						Move move = Move(x, y, last, score);

						//Add the move to the tree
						int m = add_move(group, move, false);
						//Add jumped over position to position group
						positions.positions.push_back(Position(ix, iy));

						//Check the next move
						check_move(sel, x, y, group, m, positions);
					}
				}
			}
		}

		//Check standard move
		for(int x = xs - 1; x <= xs + 1; x+=1){
			for(int y = ys - 1; y <= ys + 1; y+=1){

				Piece p = Piece(xs, ys, sel->play);
				//Check if legan
				MoveLevel m = b->isLegal(xs, ys, x, y, &p);

				if(m != MoveLevel::ILLEGAL){
					int score = ((y - ys) * up_weight) + ((last >= 0) ? (*group).move_branch[last].score : 0);
					Move move = Move(x, y, last, score);

					//Add move as leaf (since no more moves after)
					add_move(group, move, true);
				}
			}
		}
	}

	//Add a move to the tree
	int add_move(MoveGroup * group, Move move, bool top){
		if(top){
			(*group).move_leaf.push_back(move);
			return -1;
		}else{
			(*group).move_branch.push_back(move);
			return (*group).move_branch.size() - 1;
		}
	}

	void printTree(MoveGroup * g){
		for(int i = 0; i < g->move_leaf.size(); i++){
			cout << endl;
			Move m = g->move_leaf[i];
			cout << "(" << m.x << ", " << m.y << "; s:" << m.score << ") <- ";
			while(m.last > 0){
				m = g->move_branch[m.last];
				cout << "(" << m.x << ", " << m.y << "; s:" << m.score << ") <- ";
			}

			cout << "(" << g->piece->x << ", " << g->piece->y << "; s:" << m.score << ")";
		}

		cout << endl;
	}
};