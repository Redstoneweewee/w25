#include "Board.h"
#include "Errors.h"
#include "Move.h"
#include "Square.h"
#include <iostream>

enum GameState {
    NewGame,
    Continuing,
    XWin,
    OWin,
    Draw
};

void checkRules(Move lastMove, Move move);
std::string getGameText(GameState gameState, char player);
void modifyGameState(GameState& gameState, int turnNumber, char winStatus);

int main(int argc, char **argv) {
    bool verbose = false;

    if (argc == 2 && std::string(argv[1]) == "-v") {
        verbose = true;
    }

    if (verbose) {
        std::cout << "> ";
    }

    GameState gameState = NewGame;
    std::string gameText = "";
    Board board;
    Move lastMove;
    gameText = getGameText(gameState, '0');
    
    //TODO - while loop to parse each move
    while(!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        /////////////////////////std::cout << "line: [" << line << "]\n";
        if(line == "" && lastMove.number == -1) {
            continue;
        }
        if(line == "" && std::cin.eof()) {
            continue;
        }

        try {
            if((gameState == XWin || gameState == OWin) && line == "") {
                continue;
            }
            if(gameState == XWin || gameState == OWin) {
                throw InvalidMove("Invalid move. The game is finished but more moves were found.");
            }
        }
        catch(const InvalidMove &e) {
            if (verbose) {
                std::cout << "Invalid move: \n" << e.what() << '\n';
            } else {
                std::cout << "Invalid move.\n";
            }
            return 2;
        }
        //else if(line == "") {
        //    if(lastMove.player == 'X') { gameText = getGameText(gameState, 'O'); }
        //    else                       { gameText = getGameText(gameState, 'X'); }
        //    std::cout << gameText;
        //    return 0;
        //}
        //If error Move class automatically throws an error
        Move move;
        try {
            move = Move(line);
        } 
        catch(const ParseError &e) {
            if (verbose) {
                std::cout << "Parse error: \n" << e.what() << '\n';
            } else {
                std::cout << "Parse error.\n";
            }

            return 1;
        }


        try {
            checkRules(lastMove, move);
            board.tryAddMove(move.row, move.column, move.player);
        }
        catch(const InvalidMove &e) {
            if (verbose) {
                std::cout << "Invalid move: \n" << e.what() << '\n';
            } else {
                std::cout << "Invalid move.\n";
            }
            return 2;
        }

        //Use each move to fill up the board
        /////////////////////////board.printGridForTesting();
        /////////////////////////std::cout << "has won: "<< board.testForWin() << "\n";

        modifyGameState(gameState, move.number, board.testForWin());
        gameText = getGameText(gameState, move.player);
        lastMove = move;
    }

    std::cout << gameText;
    return 0;
}

void checkRules(Move lastMove, Move move) {
    //Test #1: The first move must be 1
    if(lastMove.number == -1) { 
        if(move.number != 1) { throw InvalidMove("Invalid move. The first move must be move #1."); }
        return; 
    }
    //Auto tested: nums cannot be 1< or >9
    //Test #2: Move number is correctly ordered
    if(move.number != lastMove.number + 1) { throw InvalidMove("Invalid move. Turn number is out of ourder."); }
    //Test #3: Players are alternating
    if(move.player == lastMove.player) { throw InvalidMove("Invalid move. A player went twice in a row."); }
}


void modifyGameState(GameState& gameState, int turnNumber, char winStatus) {
    if(gameState == NewGame && turnNumber > 0) {
        gameState = Continuing;
    }
    else if(gameState == Continuing && turnNumber == 9 && winStatus == '0') {
        gameState = Draw;
    }
    else if(gameState == Continuing && winStatus == 'X') {
        gameState = XWin;
    }
    else if(gameState == Continuing && winStatus == 'O') {
        gameState = OWin;
    }
}

std::string getGameText(GameState gameState, char player) {
    switch(gameState) {
        case NewGame:
            return "Game in progress: New game.\n";
            break;
        case Continuing:
            if(player == 'X') {
                return "Game in progress: O's turn.\n";
            }
            else if(player == 'O') {
                return "Game in progress: X's turn.\n";
            }
            break;
        case XWin:
            return "Game over: X wins.\n";
            break;
        case OWin:
            return "Game over: O wins.\n";
            break;
        case Draw:
            return "Game over: Draw.\n";
            break;
    }
    return "";
}
