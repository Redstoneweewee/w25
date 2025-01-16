#include "Board.h"
#include "Errors.h"
#include "Move.h"
#include "Square.h"
#include <iostream>

void checkRules(Board board);
bool testForWin();

int main() {
    enum GameState {
        NewGame,
        Continuing,
        XWin,
        OWin,
        Draw
    };
    GameState gameState = NewGame;

    //TODO - while loop to parse each move
    while(gameState == NewGame || gameState == Continuing) {
        std::string line;
        std::getline(std::cin, line);

        //If error Move class automatically throws an error
        Move move = Move(line);

        //TODO - Use each move to fill up the board
        //TODO - print out game states while happening
        //TODO - print errors when they occur
    }

    //If there's more than 9 moves, then Invalid move error
}

void checkRules(Board board) {
    //Auto tested: nums cannot be 1< or >9
    //Test #1: Move number is correctly ordered
    //Test #2: Claiming square is unclaimed
}

bool testForWin() {
    //Test #1: all horizontal
    //Test #2: all vertical
    //Test #3: both diagonals
}
