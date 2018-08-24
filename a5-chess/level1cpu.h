#ifndef LEVEL1CPU_H
#define LEVEL1CPU_H

#include "cpu.h"
#include "publicstructs.h"
#include <vector>

class ChessBoard;

class Level1CPU : public CPU {
  public:
    Level1CPU(ChessBoard &board, Colour cpuColour);
    void siftMoves(std::vector<Move> &moves) const override;
};

#endif
