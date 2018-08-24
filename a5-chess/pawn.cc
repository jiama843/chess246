#include <vector>
#include "publicstructs.h"
#include "posn.h"
#include "pawn.h"

using namespace std;

Pawn::Pawn(char piece, vector<vector<unique_ptr<ChessPiece>>> &board,
	Colour colour, Posn pos, int moveTime, bool canEnPassent) :
	ChessPiece {piece, board, colour, pos, moveTime}, canEnPassent {canEnPassent} {

}
	
void Pawn::getValidMoves(vector<Move> &movesCPU, int t) const {
	int curRow = this->pos.row;
	int curCol = this->pos.col;
	Move newDest;
	newDest.startPos = this->pos;	
	newDest.piece = this->getPiece();
	newDest.pieceColour = this->colour;
	newDest.wasRemoved = false;
	newDest.moveTime = t;
	
	if (this->colour == Colour::Black) {
		for (int i = 1; i <= 2; i++) {
			newDest.endPos.row = curRow + i;
			newDest.endPos.col = curCol;
			if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
			}
		}
		newDest.endPos.row = curRow + 1;
		newDest.endPos.col = curCol + 1;
		if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
		}
		newDest.endPos.col = curCol - 1;
		if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
		}
	}
	else {
		for (int i = 1; i <= 2; i++) {
			newDest.endPos.row = curRow - i;
			newDest.endPos.col = curCol;
			if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
			}
		}
		newDest.endPos.row = curRow - 1;
		newDest.endPos.col = curCol + 1;
		if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
		}
		newDest.endPos.col = curCol - 1;
		if (this->validMove(newDest.endPos, t)) {
				movesCPU.push_back(newDest);
		}
	}
}
	
bool Pawn::checkMove(Posn dest, int t) const {
	int curRow = this->pos.row;
	int curCol = this->pos.col;
	//ChessPiece* locDest = board[dest->row][dest->col];
	if (!(this->board[dest.row][dest.col])) {
		//checks if the pawn piece can move 1 space forward or 2 spaces if at starting position
		if (curCol == dest.col) {
			if (this->colour == Colour::Black) {
				if (dest.row == curRow + 1 || 
					(this->moveTime == 0 && dest.row == curRow + 2 && 
					 !(this->board[curRow + 1][dest.col]))) {
					return true;
				}
			}
			else {
				if (dest.row == curRow - 1 ||
					(this->moveTime == 0 && dest.row == curRow - 2 &&
					 !(this->board[curRow - 1][dest.col]))) {
					return true;
				}
			}
		}
		//checks enpassan
		else if (dest.row == curRow + 1 && 
				(dest.col == curCol + 1 || dest.col == curCol - 1)) {
			if (this->board[curRow][dest.col]) {
				if (this->colour == Colour::Black && 
					this->board[curRow][dest.col]->getMoveTime() == t - 1 &&
					this->board[curRow][dest.col]->getColour() != Colour::Black && 
					this->board[curRow][dest.col]->getPiece() == 'P' ) {
					Pawn *pawnPiece = dynamic_cast<Pawn *>(this->board[curRow][dest.col].get());
					return pawnPiece->getCanEnPassent();
				}
			}					
		}
		else if (dest.row == curRow - 1 && 
				(dest.col == curCol + 1 || dest.col == curCol - 1)) {
			if (this->board[curRow][dest.col]) {
				if (this->colour == Colour::White && 
					this->board[curRow][dest.col]->getMoveTime() == t - 1 &&
					this->board[curRow][dest.col]->getColour() != Colour::White && 
					this->board[curRow][dest.col]->getPiece() == 'P' ) {
					Pawn *pawnPiece = dynamic_cast<Pawn *>(this->board[curRow][dest.col].get());
					return pawnPiece->getCanEnPassent();
				}
			}					
		}
	}
	//checks attack/capture move
	else {
		if (curCol == dest.col + 1 || curCol == dest.col - 1) {
			if (this->colour == Colour::Black) {
				if (dest.row == curRow + 1) {
					return true;
				}
			}
			else {
				if (dest.row == curRow - 1) {
					return true;
				}
			}
		}
	}
	return false;
}

void Pawn::getCheckPosns(Posn kingPos, vector<Posn> &checkPosn) const {
	checkPosn.push_back(this->pos);
}

bool Pawn::getCanEnPassent() const {
	return this->canEnPassent;
}

