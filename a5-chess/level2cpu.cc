#include "level2cpu.h"
#include "chessboard.h"
#include "publicstructs.h"
#include "chessexception.h"
#include <utility>

using namespace std;

Level2CPU::Level2CPU(unique_ptr<CPU> cpu, ChessBoard &board, Colour cpuColour) : 
  Decorator {move(cpu), board, cpuColour} {

}

void Level2CPU::siftMoves(vector<Move> &moves) const {
    bool doesCC = false; // doesCC = does Capture or Check
    this->cpu->siftMoves(moves); 
    vector<Move> backupMoves = moves;
    Colour enemyColour = (cpuColour == Colour::Black ? Colour::White : Colour::Black);
    for(auto it = moves.begin(); it != moves.end();){

        if(this->board.doesEnemyExist(this->cpuColour, it->endPos)){
            doesCC = true;
        } else {
            try{
                this->board.doMove(it->startPos, it->endPos, this->cpuColour, false);
                doesCC = this->board.check(enemyColour);
                this->board.undo(false);
            } catch (const ChessException &ce){
                if(ce.canPawnPromote){
                    try{
                        char promotePiece = 'Q';
                        if(this->cpuColour == Colour::Black) promotePiece = 'q';
                        this->board.doPromote(it->startPos, it->endPos, this->cpuColour, promotePiece, false);
                        doesCC = this->board.check(enemyColour);
                        this->board.undo(false);
                    } catch (...) {}
                }
            }
            
        }
        if(!doesCC){
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
