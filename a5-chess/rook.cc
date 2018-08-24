#include <vector>
#include "publicstructs.h"
#include "posn.h"
#include "rook.h"

using namespace std;

Rook::Rook(char piece, vector<vector<unique_ptr<ChessPiece>>> &board,
	Colour colour, Posn pos, int moveTime) : 
	LinePiece {piece, board, colour, pos, moveTime} {

}
	
void Rook::getValidMoves(vector<Move> &movesCPU, int t) const {
	Move newDest;
	newDest.startPos = this->pos;
	newDest.piece = this->getPiece();
	newDest.pieceColour = this->colour;
	newDest.wasRemoved = false;
	newDest.moveTime = t;
	
	//checks each of the four directions cardinal
	this->tryLine(0, 1, movesCPU, newDest);
	this->tryLine(0, -1, movesCPU, newDest);
	this->tryLine(1, 0, movesCPU, newDest);
	this->tryLine(-1, 0, movesCPU, newDest);
}

bool Rook::checkMove(Posn dest, int t) const {
	int curCol = this->pos.col;
	int curRow = this->pos.row;
	
	if (curRow == dest.row || curCol == dest.col) {
		int inCol = curCol;
		int inRow = curRow;
		int colCheck = (dest.col - curCol > 0 ? 1 : -1);
		int rowCheck = (dest.row - curRow > 0 ? 1 : -1);

		// one of colCheck and rowCheck must be 0 because of precondition
		colCheck = (dest.col - curCol == 0 ? 0 : colCheck);
		rowCheck = (dest.row - curRow == 0 ? 0 : rowCheck);
		int amount = (dest.col - curCol != 0 ? 
					  dest.col - curCol : dest.row - curRow);

		amount -= (colCheck + rowCheck);

		while (amount != 0) {
			inCol += colCheck;
			inRow += rowCheck;
			if (this->board[inRow][inCol] != nullptr) {
				return false;
			}
			amount -= (colCheck + rowCheck);
		}
		return true;
	}
	return false;
}

void Rook::getCheckPosns(Posn kingPos, vector<Posn> &checkPosn) const {
	int curRow = this->pos.row;
	int curCol = this->pos.col;
	checkPosn.push_back(this->pos);

	int inCol = curCol;
	int inRow = curRow;
	int colCheck = (kingPos.col - curCol > 0 ? 1 : -1);
	int rowCheck = (kingPos.row - curRow > 0 ? 1 : -1);
	
	colCheck = (kingPos.col - curCol == 0 ? 0 : colCheck);
	rowCheck = (kingPos.row - curRow == 0 ? 0 : rowCheck);
	int amount = (kingPos.col - curCol != 0 ? 
				  kingPos.col - curCol : kingPos.row - curRow);
	
	amount -= (colCheck + rowCheck);

	while (amount != 0) {
		inCol += colCheck;
		inRow += rowCheck;
		checkPosn.push_back({inRow, inCol});
		amount -= (colCheck + rowCheck);
	}
}
