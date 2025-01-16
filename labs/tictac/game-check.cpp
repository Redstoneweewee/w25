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

int main() {
    GameState gameState = NewGame;
    std::string gameText = "";
    Board board;
    Move lastMove;
    gameText = getGameText(gameState, '0');
    
    //TODO - while loop to parse each move
    while(gameState == NewGame || gameState == Continuing) {
        std::string line;
        std::getline(std::cin, line);
        /////////////////////////std::cout << "line: [" << line << "]\n";
        if(gameState == NewGame && line == "") {
            std::cout << gameText;
            return 0;
        }
        else if(line == "") {
            if(lastMove.player == 'X') { gameText = getGameText(gameState, 'O'); }
            else                       { gameText = getGameText(gameState, 'X'); }
            std::cout << gameText;
            return 0;
        }
        //If error Move class automatically throws an error
        Move move = Move(line);

        //TODO - Use each move to fill up the board
        checkRules(lastMove, move);
        board.tryAddMove(move.row, move.column, move.player);
        /////////////////////////board.printGridForTesting();
        /////////////////////////std::cout << "has won: "<< board.testForWin() << "\n";
        modifyGameState(gameState, move.number, board.testForWin());
        gameText = getGameText(gameState, move.player);
        //TODO - print out game states while happening
        //TODO - print errors when they occur
        lastMove = move;
    }
    std::string extraLines = "";
    std::getline(std::cin, extraLines);
    if(extraLines != "") {
        throw InvalidMove("Invalid Move. The game is finished but more moves were found.");
        exit(2);
    }

    std::cout << gameText;
    return 0;
}

void checkRules(Move lastMove, Move move) {
    if(lastMove.number == -1) { return; }
    //Auto tested: nums cannot be 1< or >9
    //Test #1: Move number is correctly ordered
    if(move.number != lastMove.number + 1) { throw InvalidMove("Invalid move. Turn number is out of ourder."); exit(2); }
    //Test #2: Players are alternating
    if(move.player == lastMove.player) { throw InvalidMove("Invalid move. A player went twice in a row."); exit(2); }
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
                return "Game in progress: X's turn.\n";
            }
            else if(player == 'O') {
                return "Game in progress: O's turn.\n";
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
