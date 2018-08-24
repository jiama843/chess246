#include <vector>
#include "publicstructs.h"
#include "posn.h"
#include "queen.h"

using namespace std;


Queen::Queen(char piece, vector<vector<unique_ptr<ChessPiece>>> &board,
	Colour colour, Posn pos, int moveTime) : 
	LinePiece {piece, board, colour, pos, moveTime} {

}
	
void Queen::getValidMoves(vector<Move> &movesCPU, int t) const {
	Move newDest;
	newDest.startPos = this->pos;
	newDest.piece = this->getPiece();
	newDest.pieceColour = this->colour;
	newDest.wasRemoved = false;
	newDest.moveTime = t;

	this->tryLine(1, 1, movesCPU, newDest);
	this->tryLine(1, -1, movesCPU, newDest);
	this->tryLine(-1, 1, movesCPU, newDest);
	this->tryLine(-1, -1, movesCPU, newDest);
	this->tryLine(0, 1, movesCPU, newDest);
	this->tryLine(0, -1, movesCPU, newDest);
	this->tryLine(1, 0, movesCPU, newDest);
	this->tryLine(-1, 0, movesCPU, newDest);
}

bool Queen::checkMove(Posn dest, int t) const {
	int curCol = this->pos.col;
	int curRow = this->pos.row;
	
	if (dest.col - curCol == dest.row - curRow || dest.col - curCol == curRow - dest.row ||
		curRow == dest.row || curCol == dest.col) {
		
		int colCheck = (dest.col - curCol > 0 ? 1 : -1);
		int rowCheck = (dest.row - curRow > 0 ? 1 : -1);
		int inCol = curCol;
		int inRow = curRow;

		// if going striaght, one of colCheck and rowCheck is 0.
		// otherwise, both are non zero
		colCheck = (dest.col - curCol == 0 ? 0 : colCheck);
		rowCheck = (dest.row - curRow == 0 ? 0 : rowCheck);

		// this determines length from the piece to the target position.
		// incase the queen in going straight, it looks for the
		//  non-negative length from the piece to target position.
		//  (since going straight means either column or row doesn't change)
		int amount = (dest.col - curCol != 0 ? 
					  dest.col - curCol : dest.row - curRow);
		
		// increment amount by colCheck or rowCheck to account for
		//  the fact that the piece location is included in the amount
		//  calculation
		amount -= (dest.col - curCol != 0 ? colCheck : rowCheck);	

		while (amount != 0) {
			inRow += rowCheck;
			inCol += colCheck;
			if (this->board[inRow][inCol] != nullptr) {
				return false;
			}
			
			// because amount can be the row or column distance between 
			//  the piece and target, want to increment amount by the
			//  corresponding check integer (colCheck for column, rowCheck for row)
			amount -= (dest.col - curCol != 0 ? colCheck : rowCheck);				
		}
		return true;
	}
	return false;
}

void Queen::getCheckPosns(Posn kingPos, vector<Posn> &checkPosn) const {
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
	
	amount -= (kingPos.col - curCol != 0 ? colCheck : rowCheck);

	while (amount != 0) {
		inCol += colCheck;
		inRow += rowCheck;
		checkPosn.push_back({inRow, inCol});
		amount -= (kingPos.col - curCol != 0 ? colCheck : rowCheck);
	}
}
