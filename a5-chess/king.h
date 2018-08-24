#ifndef KING_H
#define KING_H
#include <vector>
#include <memory>
#include "chesspiece.h"


class King : public ChessPiece {
	bool checkMove(Posn dest, int t) const override; 
	
	public:
	King(char piece, std::vector <std::vector <std::unique_ptr<ChessPiece>>> &board,
		Colour colour, Posn pos, int moveTime);
	void getValidMoves(std::vector <Move> &movesCPU, int t) const override;
	void getCheckPosns(Posn kingPos, 
					   std::vector <Posn> &checkPosn) const override;
};
	
#endif
