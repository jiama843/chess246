#ifndef _LINEPIECE_H_
#define _LINEPIECE_H_
#include "chesspiece.h"
#include "publicstructs.h"
#include <memory>
#include <vector>


class LinePiece : public ChessPiece {
	protected:
        // checks all positions of the board that is in a straight path from LinePiece
        //  all valid moves that are found are incremented to movesCPU.
        // colInc and rowInc determines the straight line path it checks through.
        void tryLine(int colInc, int rowInc, std::vector<Move> &movesCPU,
                     Move &newDest) const;
    public:
        LinePiece(char piece, std::vector < std::vector <std::unique_ptr<ChessPiece>>> &board,
	        Colour colour, Posn pos, int moveTime);
};
	
#endif
