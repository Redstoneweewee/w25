#ifndef BOARD_H
#define BOARD_H

#include "Move.h"
#include "Square.h"
#include "Errors.h"
#include <vector>

// I recommended writing a Board class to manage your game state.
// Here's some space for the class definition; member functions go in Board.cpp.
class Board {
    public:
        Board();
        void tryAddMove(int row, int column, char player);
        char testForWin();
        void printGridForTesting();
    private:
        std::vector<std::vector<Square>> grid;
};

#endif
