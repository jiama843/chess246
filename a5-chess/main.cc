#include "chessboard.h"
#include "publicstructs.h"
#include "posn.h"
#include "cpu.h"
#include "level1cpu.h"
#include "level2cpu.h"
#include "level3cpu.h"
#include "level4cpu.h"
#include "chessexception.h"
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <memory>

using namespace std;

void flipColour(Colour &turnColour){
    turnColour = (turnColour == Colour::White ? Colour::Black : Colour::White);
}

void doSetup(ChessBoard &newGame, Colour &turnColour){
    static const vector<char> vPieces{'P', 'K', 'Q', 'R', 'N', 'B'};
    string input;
    cout << newGame << endl;
    while(cin >> input){
        if(input == "+"){
            char piece, row, col;
            cin >> piece >> col >> row;
            bool validPiece = false;
            for(auto validp : vPieces){
                if(toupper(piece) == validp) validPiece = true;
            }
            if(validPiece){
                Posn piecePos = ChessBoard::convertGridToPos(row, col);
                try{
                    newGame.removePiece(piecePos, true);
                    newGame.setPiece(piecePos, piece, 0, false, true);
                } catch (const ChessException &ce){
                    cout << ce.errorReason << endl;
                }
                cout << newGame << endl;
            } else {
                cout << "Make sure to add a valid piece" << endl;
            }
        } else if(input == "-"){
            char row, col;
            cin >> col >> row;
            Posn piecePos = ChessBoard::convertGridToPos(row, col);
            try{
                newGame.removePiece(piecePos, true);
            } catch (const ChessException &ce){
                cout << ce.errorReason << endl;
            }
            cout << newGame << endl;
        } else if(input == "="){
            cin >> input;
            if(input == "white"){
                turnColour = Colour::White;
            } else {
                turnColour = Colour::Black;
            }
            cout << newGame << endl;
        } else if(input == "done"){
            if(newGame.validSetup()){
                break;
            } else {
                cout << "Make sure there is exactly one king for each colour." << endl;
            }
        }
    }
}

int main () {
    double whiteScore = 0, blackScore = 0;
    string whiteType, blackType;
    Colour turnColour = Colour::White;
    ChessBoard cb{};
    unique_ptr<CPU> whiteCPU = make_unique<Level1CPU>(cb, Colour::White);
    unique_ptr<CPU> blackCPU = make_unique<Level1CPU>(cb, Colour::Black);
    bool gameFinished = true;
    bool enableUndo = false;

    string input;
    while(cin >> input){
        if(input == "game"){
            cin >> whiteType >> blackType;
            cin >> input;
            if(input == "undo"){
                enableUndo = true;
            } else {
                enableUndo = false;
            }
            cb.init();
            turnColour = Colour::White;

            if(whiteType != "human"){
                whiteCPU = make_unique<Level1CPU>(cb, Colour::White);
                if(whiteType.size() > 8){
                    switch(whiteType[8]){
                        case '4': whiteCPU = 
                                        make_unique<Level4CPU>(move(whiteCPU), cb, Colour::White);
                        case '3': whiteCPU = 
                                        make_unique<Level3CPU>(move(whiteCPU), cb, Colour::White);
                        case '2': whiteCPU = 
                                        make_unique<Level2CPU>(move(whiteCPU), cb, Colour::White);
                    }
                }
            }
            if(blackType != "human"){
                blackCPU = make_unique<Level1CPU>(cb, Colour::Black);
                if(blackType.size() > 8){
                    switch(blackType[8]){
                        case '4': blackCPU = 
                                        make_unique<Level4CPU>(move(blackCPU), cb, Colour::Black);
                        case '3': blackCPU = 
                                        make_unique<Level3CPU>(move(blackCPU), cb, Colour::Black);
                        case '2': blackCPU = 
                                        make_unique<Level2CPU>(move(blackCPU), cb, Colour::Black);
                    }
                }
            }

            cin >> input;
            if(input == "setup"){
                doSetup(cb, turnColour);
            } else {
                cb.initPieces();
            }
            cout << cb << endl;
            gameFinished = false;
            if(cb.checkmate(Colour::White)){
                cout << "Checkmate! Black Wins!" << endl;
                blackScore++;
                gameFinished = true;
            } else if(cb.checkmate(Colour::Black)){
                cout << "Checkmate! White Wins!" << endl;
                whiteScore++;
                gameFinished = true;
            } else if(cb.stalemate(turnColour)){
                cout << "Stalemate!" << endl;
                whiteScore += 0.5;
                blackScore += 0.5;
                gameFinished = true;
            } else if(cb.check(turnColour)){
                string checkColour = "White";
                if(turnColour == Colour::Black) checkColour = "Black";
                cout << checkColour << " is in check." << endl;
            }
        } else if (gameFinished){
            cout << "You must create a new game" << endl;
        } else if(input == "move"){
             if((turnColour == Colour::Black && blackType == "human") ||
                      (turnColour == Colour::White && whiteType == "human")){
                char rowStart, colStart, rowEnd, colEnd;
                cin >> colStart >> rowStart >> colEnd >> rowEnd;
                Posn start = ChessBoard::convertGridToPos(rowStart, colStart);
                Posn end = ChessBoard::convertGridToPos(rowEnd, colEnd);
                try{
                    cb.doMove(start, end, turnColour, true);
                    flipColour(turnColour);
                } catch(const ChessException &ce){
                    if(ce.canPawnPromote){
                        char promotePiece;
                        cin >> promotePiece;
                        try{
                            cb.doPromote(start, end, turnColour, promotePiece, true);
                            flipColour(turnColour);
                        } catch (const ChessException &ce){
                            cout << ce.errorReason << endl;
                        }
                    } else {
                        cout << ce.errorReason << endl;
                    }
                }
                cout << cb << endl;
            } else {
                Move cpuMove;
                if(turnColour == Colour::Black){
                    cpuMove = blackCPU->getMove();
                } else {
                    cpuMove = whiteCPU->getMove();
                }
                try{
                    cb.doMove(cpuMove.startPos, cpuMove.endPos, turnColour, true);
                    flipColour(turnColour);
                } catch (const ChessException &ce){
                    if(ce.canPawnPromote){
                        char promotePiece = 'Q';
                        if(turnColour == Colour::Black){
                            promotePiece = 'q';
                        }
                        cb.doPromote(cpuMove.startPos, cpuMove.endPos, turnColour, promotePiece, true);
                        flipColour(turnColour);
                    }
                }
                cout << cb << endl;
            }

            if(cb.isGameStalling()){
                cout << "Same board has appeared 3 or more times. Please try a different move" << endl;
            }

            if(cb.checkmate(turnColour)){
                if(turnColour == Colour::White){
                    cout << "Checkmate! Black Wins!" << endl;
                    blackScore++;
                } else {
                    cout << "Checkmate! White Wins!" << endl;
                    whiteScore++;
                }
                gameFinished = true;
            } else if(cb.stalemate(turnColour)){
                cout << "Stalemate!" << endl;
                whiteScore += 0.5;
                blackScore += 0.5;
                gameFinished = true;
            } else if(cb.check(turnColour)){
                string checkColour = "White";
                if(turnColour == Colour::Black) checkColour = "Black";
                cout << checkColour << " is in check." << endl;
            }
        } else if(input == "resign"){
            if(turnColour == Colour::White){
                cout << "Black Wins!" << endl;
                blackScore++;
            } else {
                cout << "White Wins!" << endl;
                whiteScore++;
            }
            gameFinished = true;
        } else if(input == "undo" && enableUndo){
            try{
                cb.undo(true);
                flipColour(turnColour);
            } catch (const ChessException &ce){
                cout << ce.errorReason << endl;
            }
            cout << cb << endl;
        }
    }
    cout << "Final Score:" << endl;
    cout << "White: " << whiteScore << endl;
    cout << "Black: " << blackScore << endl;
}


