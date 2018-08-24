#ifndef PAWN_H
#define PAWN_H
#include <vector>
#include <memory>
#include "chesspiece.h"


class Pawn : public ChessPiece {
	bool checkMove(Posn dest, int t) const override; 
	const bool canEnPassent;
	public:
	Pawn(char piece, std::vector <std::vector <std::unique_ptr<ChessPiece>>> &board,
		Colour colour, Posn pos, int moveTime, bool canEnPassent);
	void getValidMoves(std::vector <Move> &movesCPU, int t) const override;
	void getCheckPosns(Posn kingPos, 
					   std::vector <Posn> &checkPosn) const override;
	bool getCanEnPassent() const;
};

#endif
