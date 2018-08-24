#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <vector>
#include <memory>
#include "publicstructs.h"

class ChessPiece {
	virtual bool checkMove(Posn dest, int t) const = 0;
	const char piece;
	
	protected:
	const std::vector <std::vector <std::unique_ptr<ChessPiece>>> &board;
	const Colour colour;
	const Posn pos;
	const int moveTime;
	
	public:
	ChessPiece(char piece, std::vector<std::vector<std::unique_ptr<ChessPiece>>> &board,
		Colour colour, Posn pos, int moveTime);
	char getPiece() const;
	Colour getColour() const;
	Posn getPos() const;
	int getMoveTime() const;
	virtual ~ChessPiece();

	// validMove(Posn start, Posn dest) takes in a Posn start and Posn end and the 
	//  current time and returns if the move is legal
	bool validMove(Posn dest, int t) const;

	// getValidMoves(std::vector <Move> &movesCPU, int t) adds Move objects to
	//  movesCPU by looking through all valid moves the piece has at time t
	virtual void getValidMoves(std::vector <Move> &movesCPU, int t) const = 0;

	// getCheckPosns(Posn kingPos, std::vector <Posn> &checkPosn) assumes
	//  that the piece can already attack the enemy king. The enemy king's location
	//  is sent via kingPos. The function adds Posn objects to checkPosn which 
	//  holds all the coordinates of the path from this piece to the enemy king.
	virtual void getCheckPosns(Posn kingPos, 
					   std::vector <Posn> &checkPosn) const = 0;
};
	
#endif
