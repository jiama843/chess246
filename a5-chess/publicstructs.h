#ifndef PUBLICSTRUCTS_H
#define PUBLICSTRUCTS_H

#include "posn.h"

enum class Colour { Black, White };


struct Move{
    bool wasRemoved;
    bool canEnPassent;
    char piece;
    int moveTime;
    Colour pieceColour;
    Posn startPos;
    Posn endPos;
};


#endif
