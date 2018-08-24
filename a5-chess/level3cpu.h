#ifndef LEVEL3CPU_H
#define LEVEL3CPU_H

#include "decorator.h"
#include <vector>

class CPU;
struct Move;

class Level3CPU : public Decorator {
  public:
    Level3CPU(std::unique_ptr<CPU> cpu, ChessBoard &board, Colour cpuColour);
    void siftMoves(std::vector<Move> &moves) const override;

};

#endif
