#ifndef DECORATOR_H
#define DECORATOR_H

#include "cpu.h"
#include <memory>

class Decorator : public CPU {
  protected:
    std::unique_ptr<CPU> cpu;
  public:
    Decorator(std::unique_ptr<CPU> cpu, ChessBoard &board, Colour cpuColour);
    virtual ~Decorator();
};

#endif
