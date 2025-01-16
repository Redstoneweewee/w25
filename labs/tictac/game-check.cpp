#include "Board.h"
#include "Errors.h"
#include "Move.h"
#include <iostream>

int main() {
    enum GameState {
        NewGame,
        Continuing,
        XWin,
        OWin,
        Draw
    };
    GameState gameState;
    //TODO - while loop to parse each move
    std::string line;
    std::getline(std::cin, line);
    int moves = 0;
    while(moves <= 8) {

        //TODO - Use each move to fill up the board
        //TODO - print out game states while happening
        //TODO - print errors when they occur
        moves++;
    }

    //If there's more than 9 moves, then Invalid move error
}
