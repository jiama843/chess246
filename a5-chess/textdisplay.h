#ifndef _TEXTDISPLAY_H_
#define _TEXTDISPLAY_H_

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "publicstructs.h"

class TextDisplay {
    mutable std::map<int, std::string> displayHistory;
    std::vector<std::vector<char>> curDisplay;
    int gridSize;
    int displayTime = 0;

    void updateDisplayHistory() const;
  public:
    TextDisplay(int gridSize);
    void updateDisplay(Move newMove, int boardTime);
    bool isGameStalling();
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};


#endif
