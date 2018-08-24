#ifndef CPU_H
#define CPU_H

#include "posn.h"
#include "publicstructs.h"
#include <vector>

class ChessBoard;

class CPU {
  protected:
    ChessBoard &board; // HAS_A relationship
    Colour cpuColour;
  public:
    CPU(ChessBoard &board, Colour cpuColour);
    virtual void siftMoves(std::vector<Move> &moves) const = 0;
    Move getMove();
    virtual ~CPU();
};


#endif
