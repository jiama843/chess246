#ifndef LEVEL4CPU_H
#define LEVEL4CPU_H

#include "decorator.h"
#include "openseq.h"
#include <vector>

class CPU;
struct Move;

class Level4CPU : public Decorator {
  OpeningSeq ops;
  Move getOpeningMove(const std::vector <Move> &history) const;
  public:
    Level4CPU(std::unique_ptr<CPU> cpu, ChessBoard &board, Colour cpuColour);
    void siftMoves(std::vector<Move> &moves) const override;
};

//OpeningSeq Level4CPU::ops{};

#endif
