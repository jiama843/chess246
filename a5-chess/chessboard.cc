#include "chessboard.h"
#include "chesspiece.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include "pawn.h"
#include "publicstructs.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"
#include "chessexception.h"
#include <iostream>
#include <utility>
using namespace std;

/* Author: @John Ma
           @Taehyun Lee*/

// Helper Functions

/* createChessPiece(piece, c, end) accepts a character representing a piece
* and a colour and end position, then returns a heap allocated ChessPiece
*/

static const char nonPawnBlack [] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
static const char nonPawnWhite [] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};
static const int nonPawnLength = 8;

void ChessBoard::createChessPiece(const char piece, const Colour c,  const Posn end, 
                                  const int moveTime, const bool canEnPassent){
  if(piece == 'R'){ // Rook
    this->board.at(end.row).at(end.col) = 
          make_unique<Rook>(piece, this->board, c, end, moveTime);
  }
  else if(piece == 'N'){ // Knight
    this->board.at(end.row).at(end.col) = 
          make_unique<Knight>(piece, this->board, c, end, moveTime);
  }
  else if(piece == 'B'){ // Bishop
    this->board.at(end.row).at(end.col) = 
          make_unique<Bishop>(piece, this->board, c, end, moveTime);
  }
  else if(piece == 'P'){ // Pawn
    this->board.at(end.row).at(end.col) = 
          make_unique<Pawn>(piece, this->board, c, end, moveTime, canEnPassent);
  }
  else if(piece == 'K'){ // King
    this->board.at(end.row).at(end.col) = 
          make_unique<King>(piece, this->board, c, end, moveTime);
  }
  else { // Pawn
    this->board.at(end.row).at(end.col) = 
          make_unique<Queen>(piece, this->board, c, end, moveTime);
  }
  if(c == Colour::Black){
    this->p1.push_back(this->board.at(end.row).at(end.col).get());
  }
  else {
    this->p2.push_back(this->board.at(end.row).at(end.col).get());
  }
}


/* findAttackers(playerColour, pos) accepts a colour and a
* posn representing the colour's ChessPiece that is
* under attack. It then returns a vector containing
* all Pieces that can attack it in the following turn
*/

vector<ChessPiece *> ChessBoard::findAttackers(const Colour playerColour,
                                                         const Posn pos){
  vector<ChessPiece *> *enemyP; // Will be the opposite of current pl
  vector<ChessPiece *> attackers;

  if(playerColour == Colour::Black){
    enemyP = &(this->p2);
  }
  else {
    enemyP = &(this->p1);
  }

  try{
    for(auto c: *enemyP){
      if(validMove(c->getPos(), pos)){
        attackers.emplace_back(c);
      }
    }
  }
  catch(...){/*c doesn't exist*/}
  return attackers;
}


/* findKing(p) accepts a colour. It then returns a posn of
* it's king's location on the board
* REQUIRES: the piece exists on the board
*/
Posn ChessBoard::findKing(const Colour turnColour){
  try{
    if(turnColour == Colour::Black){ // Black pieces (p1)

        for(auto c: this->p1){
          if(c->getPiece() == 'K'){ return c->getPos(); }
        }
      }
      else{ // White Pieces (p2)

        for(auto c: this->p2){
          if(c->getPiece() == 'K'){ return c->getPos(); }
        }
      }
    }
  catch(...){}
  return Posn();
}


/* removePlayerPiece(pos) accepts a posn representing the location of
* a piece. It then removes the piece from eiither p1 or p2 by checking
* the piece's colour and determining whether or not it belongs to
* p1 (Black) or p2 (White)
*/
void ChessBoard::removePlayerPiece(const Posn pos){
  if(this->board.at(pos.row).at(pos.col)->getColour() == Colour::Black){
    for(unsigned int i = 0; i < this->p1.size(); i++){
      if(this->p1.at(i)->getPos() == pos){
        this->p1.erase(this->p1.begin() + i);
        break;
      }
    }
  }
  else{ // White Pieces (p2)

    for(unsigned int i = 0; i < this->p2.size(); i++){
      if(this->p2.at(i)->getPos() == pos){
        this->p2.erase(this->p2.begin() + i);
        break;
      }
    }
  }
}

/* checkSafe (turnColour, *c, end) accepts a colour, a piece, and a posn
* which represents the next location the piece will be moved to.
* checkSafe is never run with a piece and posn pair that will cause an
* invalidMove, since the check is done from all the functions that call it.
* checkSafe does not guarentee that the chesspiece that *c is pointing to
* will not be deleted, but it does guarentee that a new chesspiece with exact
* same values as the chesspiece *c was pointing to is recreated in the same posn
* given that there is enough heap memory. (depends on setPiece and undo
* not throwing an exception, and the only way these functions throw an exception
* is when 'new' fails in make_unique)
*/

bool ChessBoard::checkSafe(const Colour turnColour, ChessPiece *c, const Posn end){
  Posn originalPos = c->getPos();
  char pieceType = c->getPiece();
  if(c->getColour() == Colour::Black) pieceType = tolower(pieceType);
  incrementTime();

  // accounting for castling
  if(c->getPiece() == 'K'){
   if(end.col - c->getPos().col == 2){
      removePiece({c->getPos().row, c->getPos().col + 3}, false);
      char castleRook = 'R';
      if(c->getColour() == Colour::Black) castleRook = 'r';
      setPiece({c->getPos().row, c->getPos().col + 1}, castleRook,
               this->uTime, false, false);
    } else if(end.col - c->getPos().col == -2){
      removePiece({c->getPos().row, c->getPos().col - 4}, false);
      char castleRook = 'R';
      if(c->getColour() == Colour::Black) castleRook = 'r';
      setPiece({c->getPos().row, c->getPos().col - 1}, castleRook,
               this->uTime, false, false);
    }
  }

  bool canEnPassent = false;;

  // accounting for enpassent
  if(c->getPiece() == 'P'){
    if(c->getMoveTime() == 0 &&
         ((turnColour == Colour::Black && end.row - c->getPos().row == 2) ||
          (turnColour == Colour::White && end.row - c->getPos().row == -2))){
      canEnPassent = true;
    }
    if(!doesEnemyExist(turnColour, end) && c->getPos().col != end.col){
      removePiece({c->getPos().row, end.col}, false);
    }
  }
  removePiece(originalPos, false);
  removePiece(end, false);
  setPiece(end, pieceType, this->uTime, canEnPassent, false);

  if(!check(turnColour)){
    undo(false);
    return true;
  }
  undo(false);
  return false;
}


/* canBlock(pl, blockers, end) accepts a colour (turnColour), a vector of pieces that belong to
*  the colour which can block an enemy piece which has a check, and the posn of a spot that
* needs to be blocked. It returns true if moving a piece doesn't leave
* the king open to another check and false if there are no such moves
*/
bool ChessBoard::canBlock(const Colour turnColour, 
                          vector<ChessPiece *> blockers, const Posn end){
  if(blockers.size() > 0){

    vector<Posn> blockersPos;
    for(auto b : blockers){
      blockersPos.push_back(b->getPos());
    }
    // because checkSafe guarentees that after the function is run, the old piece
    //  will be deleted but a new piece with the exact specification as the old
    //  piece will be recreated in the same location, it is necessary to
    //  obtain the chesspiece by position each time checkSafe is run.
    for(auto bPos : blockersPos){
      if(checkSafe(turnColour, this->board.at(bPos.row).at(bPos.col).get(), end)){
        return true;
      }
    }
  }
  return false;
}


// Public Functions

void ChessBoard::init(){
  td = make_unique<TextDisplay>(this->size);
  gd = make_unique<GraphicsDisplay>(this->size);
  this->board.clear();
  this->p1.clear();
  this->p2.clear();
  this->uTime = 0;

  for(int i = 0; i < this->size; i++){
    vector <unique_ptr<ChessPiece>> cpv(this->size);

    for(int j = 0; j < this->size; j++){
      cpv[j].reset();
    }
    this->board.push_back(move(cpv));
  }
}


void ChessBoard::initPieces(){
  for(int i = 0; i < this->size; i++){
    setPiece({0,i}, nonPawnBlack[i], this->uTime, false, true);
    setPiece({1,i}, 'p', this->uTime, false, true);
    setPiece({7,i}, nonPawnWhite[i], this->uTime, false, true);
    setPiece({6,i}, 'P', this->uTime, false, true);
  }
}


void ChessBoard::setPiece(const Posn end, const char piece, const int moveTime,
                          const bool canEnPassent, const bool isActualMove){
  Colour pieceColour;
  char chessPiece = toupper(piece);
  if(islower(piece)){ // Black pieces (p1)
    createChessPiece(toupper(piece), Colour::Black, end, moveTime, canEnPassent);
    pieceColour = Colour::Black;
  }
  else{ // White pieces (p2)
    createChessPiece(toupper(piece), Colour::White, end, moveTime, canEnPassent);
    pieceColour = Colour::White;
  }
  
  Move m{false, canEnPassent, chessPiece, moveTime, pieceColour, end, end};
  if(isActualMove){
    td->updateDisplay(m, this->uTime);
    gd->updateDisplay(m);
  }
  history.emplace_back(m);
}


void ChessBoard::removePiece(const Posn pos, const bool isActualMove){
  try{
    if(!this->board.at(pos.row).at(pos.col)) return; //incase there is no piece at the location
  } catch (...) {
    throw ChessException{"Make sure the location you specify is in the grid", false};
  }
  ChessPiece *piece = this->board.at(pos.row).at(pos.col).get();
  char pieceType = piece->getPiece();
  int pieceTime = piece->getMoveTime();
  Colour pieceColour = piece->getColour();
  bool canEnPassent = false;

  if(pieceType == 'P'){
    Pawn *pawnPiece = dynamic_cast<Pawn *>(piece);
    canEnPassent = pawnPiece->getCanEnPassent();
  }

  removePlayerPiece(pos);
  this->board.at(pos.row).at(pos.col).reset();

  Move m{true, canEnPassent, pieceType, pieceTime, pieceColour, pos, pos};
  if(isActualMove){
    td->updateDisplay(m, this->uTime);
    gd->updateDisplay(m);
  }
  history.emplace_back(m);
}


void ChessBoard::undo(const bool isActualMove){
  if(this->uTime == 0) throw ChessException{"No more moves to undo", false};
  Move m = this->history.back();
  while(m.wasRemoved || m.moveTime == this->uTime){

    if(m.wasRemoved){
      char piece = (m.pieceColour == Colour::Black ? tolower(m.piece) : m.piece);
      setPiece(m.endPos, piece, m.moveTime, m.canEnPassent, isActualMove);
    }
    else{
      removePiece(m.endPos, isActualMove);
    }
    this->history.pop_back();
    this->history.pop_back();
    m = this->history.back();
  }
  decrementTime();
}

void ChessBoard::doMove(const Posn start, const Posn end, const Colour turnColour,
                        const bool isActualMove){
  ChessPiece *movePiece;
  try{
    movePiece = this->board.at(start.row).at(start.col).get();
    this->board.at(start.row).at(start.col);
  } catch (...) {
    throw ChessException{"Make sure the locations you specify are in the grid", false};
  }
  
  if(!movePiece || movePiece->getColour() != turnColour){
    throw ChessException{"Your first location must be your piece"};
  }
  incrementTime();

  if(!validMove(start, end)){
    decrementTime();
    throw ChessException{"Not a valid move for the specified piece", false};
  } else if (!checkSafe(turnColour, movePiece, end)){
    decrementTime();
    throw ChessException{"Cannot play a move to get in check/stay in check", false};
  } else {

    // because checkSafe actually tries the move and sees if the king is under check,
    //  the piece may have been deleted and recreated.
    // however, checkSafe undos the move after executing, so we know for sure
    //  that a new piece with the exact same specifications as the old piece
    //  has been recreated in the same spot as the old piece
    movePiece = this->board.at(start.row).at(start.col).get();
    char pieceType = movePiece->getPiece();
    if(movePiece->getColour() == Colour::Black) pieceType = tolower(pieceType);
    
    // accounting for castling
    if(movePiece->getPiece() == 'K'){
      if(end.col - start.col == 2){
        removePiece({start.row, start.col + 3}, isActualMove);
        char castleRook = 'R';
        if(movePiece->getColour() == Colour::Black) castleRook = 'r';
        setPiece({start.row, start.col + 1}, castleRook,
                 this->uTime, false, isActualMove);
      } else if(end.col - start.col == -2){
        removePiece({start.row, start.col - 4}, isActualMove);
        char castleRook = 'R';
        if(movePiece->getColour() == Colour::Black) castleRook = 'r';
        setPiece({start.row, start.col - 1}, castleRook,
                 this->uTime, false, isActualMove);
      }
    }

    bool canEnPassent = false;

    // accounting enpassent and pawn promotion
    if(movePiece->getPiece() == 'P'){
      if(movePiece->getMoveTime() == 0 &&
         ((turnColour == Colour::Black && end.row - start.row == 2) ||
          (turnColour == Colour::White && end.row - start.row == -2))){
        canEnPassent = true;
      }
      if(!doesEnemyExist(turnColour, end) && start.col != end.col){
        removePiece({start.row, end.col}, isActualMove);
      }
      if((turnColour == Colour::Black && end.row == this->size - 1) ||
         (turnColour == Colour::White && end.row == 0)){
           throw ChessException{"Promote Piece", true};
      }
    }
    removePiece(start, isActualMove);
    removePiece(end, isActualMove);
    setPiece(end, pieceType, this->uTime, canEnPassent, isActualMove);
  }
}

void ChessBoard::doPromote(const Posn start, const Posn end, const Colour turnColour,
                           const char promotePiece, const bool isActualMove){
  // no check needed, since doPromote is only called when doMove goes through
  //  but the pawn is at a location to be promoted
  bool isValidChar = false;
  for(int i = 0; i < nonPawnLength; i++){
      if(nonPawnBlack[i] == promotePiece || nonPawnWhite[i] == promotePiece){
        isValidChar = true;
      }
  }
  if((turnColour == Colour::Black && isupper(promotePiece)) ||
     (turnColour == Colour::White && islower(promotePiece))){
    throw ChessException{"Please enter a piece that is right for your colour", false};
  } else if(promotePiece == 'k' || promotePiece == 'K' || !isValidChar){
    
    throw ChessException{"Not one of the promotable pieces", false};
  }
  removePiece(start, isActualMove);
  removePiece(end, isActualMove);
  setPiece(end, promotePiece, this->uTime, false, isActualMove);
}

bool ChessBoard::isGameStalling() const{
  return td->isGameStalling();
}


bool ChessBoard::stalemate(const Colour turnColour){
  vector<ChessPiece *> currP;
  if(turnColour == Colour::Black){ currP = this->p1; }
  else{ currP = this->p2; }


  vector<Posn> currPPieces;
  for(auto c : currP){
    currPPieces.push_back(c->getPos());
  }
  for(auto cPos : currPPieces){
    vector<Move> uncheckedValidMoves;
    bool canMove = false;
    incrementTime();
    this->board.at(cPos.row).at(cPos.col)->
      getValidMoves(uncheckedValidMoves, this->uTime);
    decrementTime();

    // because checkSafe guarentees that after the function is run, the old piece
    //  will be deleted but a new piece with the exact specification as the old
    //  piece will be recreated in the same location, it is necessary to
    //  obtain the chesspiece by position each time checkSafe is run.
    for(auto move : uncheckedValidMoves){
      if(checkSafe(turnColour, this->board.at(cPos.row).at(cPos.col).get(), move.endPos)){
        canMove = true;
      }
    }
    if(canMove){
      return false;
    }
  }
  return true;
}


bool ChessBoard::checkmate(const Colour turnColour){
  Posn kingPos = findKing(turnColour);
  vector<ChessPiece *> enemyAttackers = findAttackers(turnColour, kingPos);
  Colour enemyColour = (turnColour == Colour::Black ? Colour::White : Colour::Black);

  for(int i = -1; i < 2; i++){
    for(int j = -1; j < 2; j++){
      try{
        if(this->board.at(kingPos.row + i).at(kingPos.col + j) &&
           !(i == 0 && j == 0) &&
           this->board.at(kingPos.row + i).at(kingPos.col + j)->getColour()
           == turnColour) continue;
      } catch (...){
        continue;
      }

      Posn kingNeighbour{kingPos.row + i, kingPos.col + j};
      if(findAttackers(turnColour, kingNeighbour).size() == 0){
        if(checkSafe(turnColour, this->board.at(kingPos.row).at(kingPos.col).get(),
                     kingNeighbour)){
          return false;
        }
        enemyAttackers = findAttackers(turnColour, kingPos);
      }
    }
  }

  if(enemyAttackers.size() == 1){
    vector<Posn> legalEnemyMoves;
    enemyAttackers.back()->getCheckPosns(kingPos, legalEnemyMoves);

    for(auto c: legalEnemyMoves){
      vector<ChessPiece *> blockers = findAttackers(enemyColour, c);
      if(canBlock(turnColour, blockers, c)){
        return false;
      }
      enemyAttackers = findAttackers(turnColour, kingPos);
    }

    // check if enPassent would be able to eliminate enemyAttacker
    bool canEnPassent = false;
    if(enemyAttackers.back()->getPiece() == 'P'){
      Pawn *enemyPawn = dynamic_cast<Pawn *>(enemyAttackers.back());
      canEnPassent = enemyPawn->getCanEnPassent();
    }
    if(canEnPassent){
      Posn enemyPos = enemyAttackers.back()->getPos();
      ChessPiece *left;
      ChessPiece *right;
      try{
        left = this->board.at(enemyPos.row).at(enemyPos.col - 1).get();
      } catch (...){}
      try{
        right = this->board.at(enemyPos.row).at(enemyPos.col + 1).get();
      } catch (...){}
      int enPassentIncrement = 1;
      if(turnColour == Colour::White){
        enPassentIncrement = -1;
      }

      if(left && left->getPiece() == 'P'){
        try{
          doMove(left->getPos(), {enemyPos.row + enPassentIncrement, enemyPos.col},
                 turnColour, false);
          undo(false);
          return false;
        } catch (...){}
      }
      if(right && right->getPiece() == 'P'){
        try{
          doMove(right->getPos(), {enemyPos.row + enPassentIncrement, enemyPos.col},
                 turnColour, false);
          undo(false);
          return false;
        } catch (...){}
      }
    }

    // check if castling would prevent checkmate
    if(this->board.at(kingPos.row).at(kingPos.col)->getMoveTime() == 0){
      try{
        doMove(kingPos, {kingPos.row + 2, kingPos.col}, turnColour, false);
        undo(false);
        return false;
      } catch (...){}
      try{
        doMove(kingPos, {kingPos.row - 2, kingPos.col}, turnColour, false);
        undo(false);
        return false;
      } catch (...){}
    }

  }
  return true;
}


bool ChessBoard::check(const Colour turnColour){
  Posn kingPos = findKing(turnColour);

  try{
    if(findAttackers(turnColour, kingPos).size() > 0){ return true; }
  }
  catch(...){/*Handle Exception*/}

  return false;
}


bool ChessBoard::validMove(const Posn start, const Posn end) const{
  ChessPiece *cp = this->board.at(start.row).at(start.col).get();
  return cp->validMove(end, this->uTime);
}


void ChessBoard::getValidMoves(vector<Move> &movesCPU, const Colour cpuColour){
  incrementTime();
  if(cpuColour == Colour::Black){
    for(auto piece : this->p1){
      piece->getValidMoves(movesCPU, this->uTime);
    }
  } else {
    for(auto piece : this->p2){
      piece->getValidMoves(movesCPU, this->uTime);
    }
  }
  decrementTime();
}


bool ChessBoard::isSafe(const Colour turnColour, const Posn p){
  return (findAttackers(turnColour, p).size() == 0);
}


void ChessBoard::incrementTime(){
  this->uTime++;
}


void ChessBoard::decrementTime(){
  this->uTime--;
}

Posn ChessBoard::convertGridToPos(char row, char col){
  const char startOfCol = 'a';
  const char maxRow = '8';
  return {maxRow - row, col - startOfCol};
}


bool ChessBoard::doesEnemyExist(const Colour turnColour, const Posn p){
  try{
    if(this->board.at(p.row).at(p.col) &&
       this->board.at(p.row).at(p.col)->getColour() != turnColour){
      return true;
    }
  } catch (...) {

  }
  return false;
}

const vector<Move> &ChessBoard::getHistory(){
  return this->history;
}

bool ChessBoard::validSetup(){
  int whiteKingNum = 0, blackKingNum = 0;
  for(auto piece : this->p1){
    if(piece->getPiece() == 'K') blackKingNum++;
  }
  for(auto piece : this->p2){
    if(piece->getPiece() == 'K') whiteKingNum++;
  }
  return (whiteKingNum == 1 && blackKingNum == 1);
}


std::ostream &operator<<(std::ostream &out, const ChessBoard &cb){
  out << *(cb.td);
  return out;
}
