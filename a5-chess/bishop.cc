#include <vector>
#include "publicstructs.h"
#include "posn.h"
#include "bishop.h"

using namespace std;

Bishop::Bishop(char piece, vector<vector<unique_ptr<ChessPiece>>> &board,
	Colour colour, Posn pos, int moveTime) :
	LinePiece {piece, board, colour, pos, moveTime} {

}

void Bishop::getValidMoves(vector<Move> &movesCPU, int t) const {
	Move newDest;
	newDest.startPos = this->pos;
	newDest.piece = this->getPiece();
	newDest.pieceColour = this->colour;
	newDest.wasRemoved = false;
	newDest.moveTime = t;

	//checks each of the four intercardinal directions
	this->tryLine(1, 1, movesCPU, newDest);
	this->tryLine(1, -1, movesCPU, newDest);
	this->tryLine(-1, 1, movesCPU, newDest);
	this->tryLine(-1, -1, movesCPU, newDest);
}

bool Bishop::checkMove(Posn dest, int t) const {
	int curCol = this->pos.col;
	int curRow = this->pos.row;
	if (dest.col - curCol == dest.row - curRow || dest.col - curCol == curRow - dest.row) {
		// Make sure that all intervening squares are empty
		int inRow = curRow;
		int inCol = curCol;
		int rowCheck = (dest.row - curRow > 0 ? 1 : -1);
		int colCheck = (dest.col - curCol > 0 ? 1 : -1);
		int amount = dest.row - curRow - rowCheck;

		while (amount != 0) {
			inRow += rowCheck;
			inCol += colCheck;
			if (this->board[inRow][inCol] != nullptr) {
				return false;
			}
			amount -= rowCheck;
		}
		return true;
	}
	return false;
}

void Bishop::getCheckPosns(Posn kingPos, vector<Posn> &checkPosn) const {
	int curRow = this->pos.row;
	int curCol = this->pos.col;
	checkPosn.push_back(this->pos);

	int rowCheck = (kingPos.row - curRow > 0 ? 1 : -1);
	int colCheck = (kingPos.col - curCol > 0 ? 1 : -1);
	int amount = kingPos.row - curRow - rowCheck;
	int inRow = curRow;
	int inCol = curCol;
	while (amount != 0) {
		inRow += rowCheck;
		inCol += colCheck;
		amount -= rowCheck;
		checkPosn.push_back({inRow, inCol});
	}
}
