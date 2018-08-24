#ifndef _GRAPHICSDISPLAY_H_
#define _GRAPHICSDISPLAY_H_

#include "window.h"
#include <memory>
#include "publicstructs.h"

/* author: @John Ma*/

class GraphicsDisplay {
  int gridSize; // Will equal 8 + 1 = 9, since the 0the rows are reserved for numbers
  int tileSize;
  std::unique_ptr<Xwindow> xwin;
public:
  GraphicsDisplay(int gridSize);
  void updateDisplay(Move newMove);
};

#endif
