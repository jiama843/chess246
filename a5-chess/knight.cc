#include <vector>
#include "publicstructs.h"
#include "posn.h"
#include "knight.h"

using namespace std;


Knight::Knight(char piece, vector<vector<unique_ptr<ChessPiece>>> &board,
	Colour colour, Posn pos, int moveTime) : 
	ChessPiece {piece, board, colour, pos, moveTime} {

}
void Knight::getValidMoves(vector<Move> &movesCPU, int t) const {
	int curCol = this->pos.col;
	int curRow = this->pos.row;
	Move newDest;
	newDest.startPos = this->pos;
	newDest.piece = this->getPiece();
	newDest.pieceColour = this->colour;
	newDest.wasRemoved = false;
	newDest.moveTime = t;
	
	//up/down one, left/right two
	newDest.endPos.row = curRow + 1;
	newDest.endPos.col = curCol + 2;
	if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
	}
	newDest.endPos.col = curCol - 2;
	if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
	}
	newDest.endPos.row = curRow - 1;
	newDest.endPos.col = curCol + 2;
	if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
	}
	newDest.endPos.col = curCol - 2;
	if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
	}
	
	//up/down two, left/right one
	newDest.endPos.row = curRow + 2;
	newDest.endPos.col = curCol + 1;
	if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
	}
	newDest.endPos.col = curCol - 1;
	if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
	}
	newDest.endPos.row = curRow - 2;
	newDest.endPos.col = curCol + 1;
	if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
	}
	newDest.endPos.col = curCol - 1;
	if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
	}
}

bool Knight::checkMove(Posn dest, int t) const {
	int curCol = this->pos.col;
	int curRow = this->pos.row;

	if ((curRow == dest.row + 2 || curRow == dest.row - 2) &&
	 	(curCol == dest.col + 1 || curCol == dest.col - 1)){
		return true;
	}
	else if ((curRow == dest.row + 1 || curRow == dest.row - 1) &&
	 		 (curCol == dest.col + 2 || curCol == dest.col - 2)){
		return true;
	}
	return false;
}

void Knight::getCheckPosns(Posn kingPos, vector<Posn> &checkPosn) const {
	checkPosn.push_back(this->pos);
}
