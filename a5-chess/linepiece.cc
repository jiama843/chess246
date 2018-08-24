#include "linepiece.h"
#include "posn.h"

using namespace std;

LinePiece::LinePiece(char piece, vector<vector<unique_ptr<ChessPiece>>> &board,
	Colour colour, Posn pos, int moveTime) :
    ChessPiece {piece, board, colour, pos, moveTime} {

}

void LinePiece::tryLine(int colInc, int rowInc, vector<Move> &movesCPU, Move &newDest) const{
    for (int i = 1; i < 8; i++) {
		newDest.endPos.row = this->pos.row + (i * rowInc);
		newDest.endPos.col = this->pos.col + (i * colInc);
		if (this->validMove(newDest.endPos, newDest.moveTime)) {
				movesCPU.push_back(newDest);
		}
		else {
			break;
		}
	}
}


