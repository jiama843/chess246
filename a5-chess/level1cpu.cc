#include "level1cpu.h"
#include "chessexception.h"
#include "chessboard.h"
using namespace std;

Level1CPU::Level1CPU(ChessBoard &board, Colour cpuColour) : 
    CPU {board, cpuColour} {

}

void Level1CPU::siftMoves(vector<Move> &moves) const{

    bool isLegal = true;

    for(auto it = moves.begin(); it != moves.end();){

        // accounts for shift of iterator when an invalid move is erased
        try{
            this->board.doMove(it->startPos, it->endPos, this->cpuColour, false);
            isLegal = true;
            this->board.undo(false);
        } catch (const ChessException &ce){
            isLegal = ce.canPawnPromote;
        }
        if(!isLegal){
            it = moves.erase(it);
        } else {
            ++it;
        }
    }
}
