#include "posn.h"

bool Posn::operator==(const Posn &other){
  return ((this->col == other.col) && (this->row == other.row));
}

bool Posn::operator!=(const Posn &other){
  return !(*this == other);
}
