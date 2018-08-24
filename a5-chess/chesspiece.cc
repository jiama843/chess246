#include <string>
#include "chesspiece.h"
#include "posn.h"
// #include "chessboard.h" consider forward declare
using namespace std;


ChessPiece::ChessPiece(char piece, vector<vector<unique_ptr<ChessPiece>>> &board,
	Colour colour, Posn pos, int moveTime) : piece {piece}, board {board},
	colour {colour}, pos {pos}, moveTime {moveTime} {

}
	
//getPiece() returns the current piece type depending on color
char ChessPiece::getPiece() const {
	return this->piece;
}
	
	//getColor() Returns the current colour (W/B)
Colour ChessPiece::getColour() const {
	return this->colour;
}

bool ChessPiece::validMove(Posn dest, int t) const {
	ChessPiece *locDest = nullptr;
	try{
		locDest = this->board.at(dest.row).at(dest.col).get();
	} catch (...){
		return false;
	}
	bool vMove = false;
	if ((locDest == nullptr || this->colour != locDest->getColour()) && 
		 dest != this->pos) {
		vMove = checkMove(dest, t);
	}
	return vMove;
}
	
	//getPos() returns the current position of the piece as a Posn
Posn ChessPiece::getPos() const {
	return this->pos;
}
	
	//getTime() returns the current time of the piece as an int
int ChessPiece::getMoveTime() const {
	return this->moveTime;
}

ChessPiece::~ChessPiece(){
	
}



