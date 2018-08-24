#include "openseq.h"
#include <fstream>
#include <cctype>
using namespace std;

/* author: @John Ma*/

/* contains(vm1, vm2) accepts two vectors of moves and prints true if
* vm1 is a subset of vm2, otherwise it prints false
*/
bool OpeningSeq::contains(const vector<Move> &vm1, const vector<Move> &vm2) const{
  try{
    for(unsigned int i = setupHist; i < vm1.size(); i++){
      if(vm1.at(i).wasRemoved != vm2.at(i - setupHist).wasRemoved ||
         vm1.at(i).piece != vm2.at(i - setupHist).piece ||
         vm1.at(i).endPos.row != vm2.at(i - setupHist).endPos.row ||
         vm1.at(i).endPos.col != vm2.at(i - setupHist).endPos.col){ return false; }
    }
  }
  catch(...){ return false; }
  return true;
}


OpeningSeq::OpeningSeq(){
  ifstream ifs("openingmoves.txt");

  int seqLen;

  while(ifs >> seqLen){

    vector <Move> vm;
    bool wasRemoved, canEnPassent;
    char piece;
    int moveTime;
    Posn movePos;

    for(int i = 0; i < seqLen; i++){
      ifs >> boolalpha >> wasRemoved;
      ifs >> boolalpha >> canEnPassent;
      ifs >> piece;
      ifs >> moveTime;
      ifs >> movePos.row >> movePos.col;

      Colour c = Colour::White;
      if(islower(piece)){ c = Colour::Black; }

      char p = toupper(piece);

      Move m{wasRemoved, canEnPassent, p, moveTime, c, movePos, movePos};
      vm.emplace_back(m);
    }
    this->sequences.push_back(vm);
  }
}


Move OpeningSeq::nextMove(const vector<Move> &history) const{

  vector <vector<Move>> seq;

  int histLen = history.size() - setupHist;

  for(auto c: this->sequences){
    if(contains(history, c)){
      seq.emplace_back(c);
    }
  }

  int index = seq.size() != 0? rand() % seq.size(): seq.size();
  Move seqMove;
  Posn endPos;
  try{
    seqMove = seq.at(index).at(histLen);
    endPos = seq.at(index).at(histLen + 1).endPos;
  } catch(...){ 
    // if above fails, it means no more opening sequence moves available
    //  so send a move that is impossible to do, in which case
    //  Level4CPU will try the illegal move and quickly catch
    seqMove.startPos = Posn{-1, -1};
    endPos = Posn{-1, -1};
  }

  Move next{false, false, seqMove.piece, seqMove.moveTime, seqMove.pieceColour, seqMove.startPos, endPos};

  return next;
}
