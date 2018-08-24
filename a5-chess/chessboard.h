#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <cctype>
#include <iostream>
#include <memory>
#include "publicstructs.h"
#include "posn.h"
#include "chesspiece.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"

/* Author: @John Ma */

class ChessBoard {
    std::vector <std::vector <std::unique_ptr<ChessPiece>>> board;
    std::vector<ChessPiece *> p1; // Black Pieces
    std::vector<ChessPiece *> p2; // White Pieces
    std::vector<Move> history;
    int uTime = 0;
    int size = 8;
    std::unique_ptr<TextDisplay> td; // Pointer to TextDisplay
    std::unique_ptr<GraphicsDisplay> gd; // Pointer to GraphicsDisplay

    void removePlayerPiece(const Posn pos);
    void incrementTime(); // Increases uTime by 1
    void decrementTime(); // Decreases uTime by 1
    bool checkSafe(const Colour turnColour, ChessPiece *c,const Posn end);
    bool canBlock(const Colour turnColour, std::vector<ChessPiece *> blockers, const Posn end);
    void createChessPiece(const char piece, const Colour c, const Posn end, 
                                 const int moveTime, const bool canEnPassent);
    std::vector<ChessPiece *> findAttackers(const Colour playerColour, const Posn pos);
    Posn findKing(const Colour turnColour);

  public:
    void init(); // Generates the board
    void initPieces(); // Generates default pieces on the board
    void setPiece(const Posn end, const char piece, const int moveTime, 
                  const bool canEnPassent, const bool isActualMove); // Sets a piece in posn end
    void removePiece(const Posn pos, const bool isActualMove); // Removes a piece in posn pos
    void undo(const bool isActualMove); // Undoes the last move
    void doMove(const Posn start, const Posn end, const Colour turnColour,
                 const bool isActualMove); // tries to move the piece from start to end 
                                           //  position and returns error if the movement 
                                           //  is illegal
    void doPromote(const Posn start, const Posn end, const Colour turnColour,
                   const char promotePiece, const bool isActualMove); // promotes pawn at start location to 
                                                                      //  promotePiece and places it at end location
    void getValidMoves(std::vector<Move> &movesCPU,const Colour cpuColour); // updates given movesCPU vector
                                                                       //  by calling getValidMoves
                                                                       //  to respective Player pieces
    bool stalemate(const Colour turnColour); // Checks if the current board state is in stalemate for player
    bool checkmate(const Colour turnColour); // Checks if the current board state is in checkmate for player
    bool check(const Colour turnColour); // Checks if the player is in check
    bool validMove(const Posn start, const Posn end) const; // Checks if current move is valid
    bool isGameStalling() const;
    bool doesEnemyExist(const Colour turnColour, const Posn p); //Checks if there is an enemy at the specified location
    bool isSafe(const Colour turnColour, const Posn p); // Returns true if enemy pieces can move to the isSafe location
                                                        //  false otherwise
    bool validSetup(); // Checks if the board has valid setup (i.e. each player has at least exactly one king)
    static Posn convertGridToPos(char row, char col);
    const std::vector<Move> &getHistory();
    friend std::ostream &operator<<(std::ostream &out, const ChessBoard &cb);
};

#endif
