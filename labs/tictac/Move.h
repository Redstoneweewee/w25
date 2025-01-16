#ifndef MOVE_H
#define MOVE_H

// This file defines a Move structure and some helper functions.
// You can edit it if you feel like it, but you shouldn't need to.
// You're free to interpret the member variables as you wish,
// but you must print moves in the expected format.

#include <string>
#include <vector>

struct Move {
    // Member Variables
    int number;
    char player;
    int row;
    int column;

    // The constructor parses a string.
    Move();
    Move(const std::string& input);

    // Helper to render a Move as text.
    std::string to_string() const;

    //Helper functions
    private:
        char rowToString() const;
        int rowCharToInt(char c) const;
        void checkAttributes(std::vector<char> attributesAsChars) const;
        std::vector<char> parseStringIntoAttributesAsChar(std::string input) const;
};

#endif
