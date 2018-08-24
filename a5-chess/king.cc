#include <vector>
#include <algorithm>
#include "publicstructs.h"
#include "posn.h"
#include "king.h"

using namespace std;
King::King(char piece, vector<vector<unique_ptr<ChessPiece>>> &board,
	Colour colour, Posn pos, int moveTime) :
	ChessPiece {piece, board, colour, pos, moveTime} {

}

void King::getValidMoves(vector<Move> &movesCPU, int t) const {
	int curRow = this->pos.row;
	int curCol = this->pos.col;
	Move newDest;
	newDest.startPos = this->pos;
	newDest.piece = this->getPiece();
	newDest.pieceColour = this->colour;
	newDest.wasRemoved = false;
	newDest.moveTime = t;
	
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			newDest.endPos.row = curRow + i;
			newDest.endPos.col = curCol + j;
			if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
			}
		}
	}
	//check castle
	newDest.endPos.row = curRow;
	newDest.endPos.col = curCol;
	if (this->validMove(newDest.endPos, t)) {
						movesCPU.push_back(newDest);
	}
	newDest.endPos.col = curCol - 2;
	if (this->validMove(newDest.endPos, t)) {
						movesCPU.push_back(newDest);
	}
}

bool King::checkMove(Posn dest, int t) const {
	int curRow = this->pos.row;
	int curCol = this->pos.col;
	int changeRow = dest.row - curRow;
	int changeCol = dest.col - curCol;
	if ((changeRow <= 1 && changeRow >= -1) && (changeCol <= 1 && changeCol >= -1)) {
		return true;
	}
	else {
		if (this->moveTime == 0) {
		//castle short
			if (changeCol == 2 && changeRow == 0) {
				if (!(this->board[curRow][curCol + 1]) && 
					!(this->board[curRow][curCol + 2])){
					ChessPiece *edgePiece = this->board[curRow][curCol + 3].get();
					if (edgePiece && edgePiece->getPiece() == 'R' && 
						edgePiece->getMoveTime() == 0 &&
						edgePiece->getColour() == this->colour){
						return true;
					}
				}

			}
		//castle long
			else if (changeCol == -2 && changeRow == 0) {
				if (!(this->board[curRow][curCol - 1]) && 
					!(this->board[curRow][curCol - 2]) &&
					!(this->board[curRow][curCol - 3])) {
					ChessPiece *edgePiece = this->board[curRow][curCol - 4].get();
					if (edgePiece && edgePiece->getPiece() == 'R' && 
						edgePiece->getMoveTime() == 0 &&
						edgePiece->getColour() == this->colour){
						return true;
					}
				}
			}
		}
	}
	return false;
}

void King::getCheckPosns(Posn kingPos, vector<Posn> &checkPosn) const {
	checkPosn.push_back(this->pos);
}
