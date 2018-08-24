#include "textdisplay.h"
#include "posn.h"
#include <cctype>
#include <utility>
using namespace std;


TextDisplay::TextDisplay(int gridSize) : gridSize {gridSize} {
    for(int i = 0; i < this->gridSize; i++){
        this->curDisplay.push_back(vector<char>());
        for(int j = 0; j < this->gridSize; j++){
            char tile = ' ';
            if((i + j) % 2 != 0) tile = '-';
            this->curDisplay.at(i).push_back(tile);
        }
    }
}

void TextDisplay::updateDisplayHistory() const{
    this->displayHistory[this->displayTime] = "";
    for(auto row : this->curDisplay){
        for(auto tile : row){
            this->displayHistory[this->displayTime] += tile;
        }
    }
}

void TextDisplay::updateDisplay(Move newMove, int boardTime){
    char tile = ' ';
    if((newMove.endPos.row + newMove.endPos.col) % 2 != 0) tile = '-';
    this->curDisplay.at(newMove.endPos.row).at(newMove.endPos.col) = tile;
    if(!newMove.wasRemoved){
        char piece = newMove.piece;
        if(newMove.pieceColour == Colour::Black) piece = tolower(piece);
        this->curDisplay.at(newMove.endPos.row).at(newMove.endPos.col) = piece;
    }
    if(boardTime > this->displayTime){
        this->displayTime++;
    } else if(boardTime < this->displayTime){
        this->displayHistory.erase(this->displayTime - 1);
        this->displayTime--;
    }
}

bool TextDisplay::isGameStalling(){
    int repCount = 1;
    for(int i = this->displayTime - 1; i >= 0; i--){
        if(this->displayHistory[i] == this->displayHistory[this->displayTime]){
            repCount++;
        }
        if(repCount >= 3){
            return true;
        }
    }
    return false;
}

ostream &operator<<(ostream &out, const TextDisplay &td){
    td.updateDisplayHistory();
    for(int i = 0; i < td.gridSize; i++){
        out << td.gridSize - i << ' ';
        for(auto tiles : td.curDisplay.at(i)){
            out << tiles;
        }
        out << endl;
    }
    out << endl;
    out << "  ";
    for(char i = 'a'; i < 'a' + td.gridSize; i++){
        out << i;
    }
    return out;
}
