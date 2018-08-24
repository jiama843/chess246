#include "level4cpu.h"
#include "chessboard.h"
#include "publicstructs.h"
#include "chessexception.h"
#include <utility>
//#include <iostream>
using namespace std;

Move Level4CPU::getOpeningMove(const vector <Move> &history) const{
  return ops.nextMove(history);
}


Level4CPU::Level4CPU(unique_ptr<CPU> cpu, ChessBoard &board, Colour cpuColour) :
  Decorator {move(cpu), board, cpuColour} {}


void Level4CPU::siftMoves(vector<Move> &moves) const {
    bool openingMoveExist = false;
    this->cpu->siftMoves(moves);
    vector<Move> backupMoves = moves;
    Move openingMove;//{};
    try{
        openingMove = this->getOpeningMove(this->board.getHistory());
        openingMoveExist = true;
    } catch (...){}

    try{
        this->board.doMove(openingMove.startPos, openingMove.endPos,
                           this->cpuColour, false);
        this->board.undo(false);
    } catch (const ChessException &ce){
        openingMoveExist = false;
        //cerr << ce.errorReason << endl;
    }
    
    if(openingMoveExist){
        moves.clear();
        moves.push_back(openingMove);
    } else {
        for(auto it = moves.begin(); it != moves.end();){
            // prioritises move on pieces that are already under attack
            if(this->board.isSafe(this->cpuColour, it->startPos)){
                it = moves.erase(it);
            } else {
                ++it;
            }
        }
    }

    // to make sure that at least some moves are being returned
    if(moves.size() == 0){
        moves = backupMoves;
    }
}
