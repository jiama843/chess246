#ifndef QUEEN_H
#define QUEEN_H
#include <vector>
#include "linepiece.h"

class Queen : public LinePiece {
	bool checkMove(Posn dest, int t) const override; 
	
	public:
	Queen(char piece, std::vector <std::vector <std::unique_ptr<ChessPiece>>> &board,
		Colour colour, Posn pos, int moveTime);
	void getValidMoves(std::vector <Move> &movesCPU, int t) const override;
	void getCheckPosns(Posn kingPos, 
					   std::vector <Posn> &checkPosn) const override;
};
	
	
#endif
