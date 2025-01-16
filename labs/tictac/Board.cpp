#include "Errors.h"
#include "Board.h"
#include "Square.h"
#include <iostream>
#include <vector>

// Space for implementing Board functions.


Board::Board() {
    std::vector<Square> row = {Unclaimed, Unclaimed, Unclaimed};
    for(unsigned int i=0; i<3; i++) {
        grid.push_back(row);
    }
}

void Board::printGridForTesting() {
    
    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {
            if(grid[i][j] == Unclaimed) {
                std::cout << "_ ";
            }
            else if(grid[i][j] == X) {
                std::cout << "X ";
            }
            else if(grid[i][j] == O) {
                std::cout << "O ";
            }
            else {
                std::cout << "? ";
            }
        }
        std::cout << "\n";
    }
}


void Board::tryAddMove(int row, int column, char player) {
    if(grid[row-1][column-1] != Unclaimed) {
        throw InvalidMove("Invalid Move. Tried to claim a claimed square.");
        exit(2);
    }
    
    if(player == 'X') {
        grid[row-1][column-1] = X;
    }
    else {
        grid[row-1][column-1] = O;
    }
}


//Returns '0' if nobody has won yet, 'X' if X wins, 'O' if O wins
char Board::testForWin() {
    int XCount = 0;
    int OCount = 0;

    //Test #1: all horizontal
    for(unsigned int i=0; i<3; i++) {
        XCount = 0;
        OCount = 0;
        for(unsigned int j=0; j<3; j++) {
            if(grid[i][j] == X) {
                XCount++;
            }
            else if(grid[i][j] == O) {
                OCount++;
            }
        }

        if(XCount == 3) {
            return 'X';
        }
        else if(OCount == 3) {
            return 'O';
        }
    }

    //Test #2: all vertical
    for(unsigned int i=0; i<3; i++) {
        XCount = 0;
        OCount = 0;
        for(unsigned int j=0; j<3; j++) {
            if(grid[j][i] == X) {
                XCount++;
            }
            else if(grid[j][i] == O) {
                OCount++;
            }
        }

        if(XCount == 3) {
            return 'X';
        }
        else if(OCount == 3) {
            return 'O';
        }
    }

    //Test #3: both diagonals
    XCount = 0;
    OCount = 0;
    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {
            if(i != j) { continue; }
            if(grid[i][j] == X) {
                XCount++;
            }
            else if(grid[i][j] == O) {
                OCount++;
            }
        }
    }
    if(XCount == 3) {
        return 'X';
    }
    else if(OCount == 3) {
        return 'O';
    }



    XCount = 0;
    OCount = 0;
    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {
            if(i + j != 2) { continue; }
            if(grid[i][j] == X) {
                XCount++;
            }
            else if(grid[i][j] == O) {
                OCount++;
            }
        }
    }
    if(XCount == 3) {
        return 'X';
    }
    else if(OCount == 3) {
        return 'O';
    }

    return '0';
}