#include "cpu.h"
#include "decorator.h"
#include <utility>

using namespace std;

Decorator::Decorator(unique_ptr<CPU> cpu, ChessBoard &board, Colour cpuColour) : 
  CPU {board, cpuColour}, cpu {move(cpu)} {

}

Decorator::~Decorator(){
  
}

