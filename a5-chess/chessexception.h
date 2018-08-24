#ifndef CHESSEXCEPTION_H
#define CHESSEXCEPTION_H
#include <string>

struct ChessException {
  std::string errorReason;
  bool canPawnPromote;
};

#endif
