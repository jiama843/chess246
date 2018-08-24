#ifndef OPENINGSEQ_H
#define OPENINGSEQ_H

#include <vector>
#include "publicstructs.h"

/* author: @John Ma*/

class OpeningSeq{
  std::vector <std::vector<Move>> sequences;
  int setupHist = 32; //setup moves
  bool contains(const std::vector<Move> &vm1, const std::vector<Move> &vm2) const;
public:
  OpeningSeq();
  Move nextMove(const std::vector<Move> &history) const;
};

#endif
