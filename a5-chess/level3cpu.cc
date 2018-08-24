#include "level3cpu.h"
#include "chessboard.h"
#include "chessexception.h"
#include "publicstructs.h"
#include <utility>

using namespace std;

Level3CPU::Level3CPU(unique_ptr<CPU> cpu, ChessBoard &board, Colour cpuColour) : 
  Decorator {move(cpu), board, cpuColour} {

}

void Level3CPU::siftMoves(vector<Move> &moves) const {
    bool safeMove = true;
    this->cpu->siftMoves(moves); 
    vector<Move> backupMoves = moves;
    for(auto it = moves.begin(); it != moves.end();){
        try{
            this->board.doMove(it->startPos, it->endPos, this->cpuColour, false);
            safeMove = this->board.isSafe(this->cpuColour, it->endPos);
            this->board.undo(false);
        } catch(const ChessException &ce){
            if(ce.canPawnPromote){
                try{
                    char promotePiece = 'Q';
                    if(this->cpuColour == Colour::Black) promotePiece = 'q';
                    this->board.doPromote(it->startPos, it->endPos, this->cpuColour, promotePiece, false);
                    safeMove = this->board.isSafe(this->cpuColour, it->endPos);
                    this->board.undo(false);
                } catch (...) {}
            }
        }
        
        if(!safeMove){
            it = moves.erase(it);
        } else {
            ++it;
        }
    }

    // to make sure that at least some moves are being returned
    if(moves.size() == 0){
        moves = backupMoves;
    }
}
