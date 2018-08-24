#ifndef LEVEL2CPU_H
#define LEVEL2CPU_H

#include "decorator.h"
#include <vector>

class CPU;
struct Move;

class Level2CPU : public Decorator {
  public:
    Level2CPU(std::unique_ptr<CPU> cpu, ChessBoard &board, Colour cpuColour);
    void siftMoves(std::vector<Move> &moves) const override;

};

#endif
