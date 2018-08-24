#include "graphicsdisplay.h"
#include <string>

using namespace std;

/* author: @John Ma*/

GraphicsDisplay::GraphicsDisplay(int gridSize)
: gridSize{gridSize}, tileSize{500 / (gridSize + 1)}, xwin {make_unique<Xwindow>()}{

  this->gridSize++;
  this->xwin->fillRectangle(0, 0, 1000, 1000, Xwindow::Blue);

  for(int i = 0; i < this->gridSize; i++){
      for(int j = 0; j < this->gridSize; j++){

          if(i == 0){
            this->xwin->drawString(i * this->tileSize, j * this->tileSize, 
                                   to_string(this->gridSize - j), Xwindow::White);
            continue;
          }

          if(j == 8){
            char c = ('a' + (i - 1));
            this->xwin->drawString(i * this->tileSize, (j + 1) * this->tileSize, 
                                   string(1, c), Xwindow::White);
            continue;
          }

          this->xwin->fillRectangle(i * this->tileSize, j * this->tileSize, 
                                    this->tileSize, this->tileSize, Xwindow::Green);
          if((i + j) % 2 != 0){ 
           this->xwin->fillRectangle(i * this->tileSize, j * this->tileSize, 
                                     this->tileSize, this->tileSize, Xwindow::Red); 
          }
    }
  }
}



void GraphicsDisplay::updateDisplay(Move newMove){

  if(newMove.wasRemoved){

    if((newMove.startPos.col + newMove.startPos.row) % 2 != 0){
      this->xwin->fillRectangle((newMove.endPos.col + 1) * this->tileSize, 
                                 newMove.endPos.row * this->tileSize, this->tileSize, 
                                 this->tileSize, Xwindow::Green);
    }
    else{
      this->xwin->fillRectangle((newMove.endPos.col + 1) * this->tileSize, 
                                 newMove.endPos.row * this->tileSize, this->tileSize, 
                                 this->tileSize, Xwindow::Red);
    }
  }
  else{

    if(newMove.pieceColour == Colour::Black){
      this->xwin->fillRectangle((newMove.endPos.col + 1) * this->tileSize + 5, 
                                 newMove.endPos.row * this->tileSize + 5, this->tileSize - 10, 
                                 this->tileSize - 10, Xwindow::Black);
      this->xwin->drawString((newMove.endPos.col + 1) * this->tileSize + this->tileSize / 2, 
                             (newMove.endPos.row + 1) * this->tileSize - this->tileSize / 2, 
                             string(1, toupper(newMove.piece)), Xwindow::White);
    }
    else{
      this->xwin->fillRectangle((newMove.endPos.col + 1) * this->tileSize + 5, 
                                 newMove.endPos.row * this->tileSize + 5, this->tileSize - 10, 
                                 this->tileSize - 10, Xwindow::White);
      this->xwin->drawString((newMove.endPos.col + 1) * this->tileSize + this->tileSize / 2, 
                             (newMove.endPos.row + 1) * this->tileSize - this->tileSize / 2, 
                             string(1, newMove.piece), Xwindow::Black);
    }
  }
}

