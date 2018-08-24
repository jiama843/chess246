#include "cpu.h"
#include "chessboard.h"
#include <cstdlib>
#include <iostream>

using namespace std;


CPU::CPU(ChessBoard &board, Colour cpuColour): 
    board {board}, cpuColour {cpuColour} {

}

Move CPU::getMove(){
    vector<Move> allValidMoves;
    this->board.getValidMoves(allValidMoves, this->cpuColour);
    vector<Move> backUpMoves = allValidMoves;
    siftMoves(allValidMoves);
    if(allValidMoves.size() == 0) allValidMoves = backUpMoves;
    int randomIndex = rand() % allValidMoves.size();
    return allValidMoves.at(randomIndex);
}

CPU::~CPU(){

}
