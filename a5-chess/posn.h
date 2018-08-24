#ifndef POSN_H
#define POSN_H

struct Posn {
    int row;
    int col;
    bool operator==(const Posn &other);
    bool operator!=(const Posn &other);
};

#endif
