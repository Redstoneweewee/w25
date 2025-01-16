#include "Errors.h"
#include "Move.h"
#include <vector>
#include <string>
#include <cctype>

//for testing purposes
#include <iostream>
//

// Space for implementing Move functions.
Move::Move(const std::string& input) {
    std::vector<char> attributesAsChars = parseStringIntoAttributesAsChar(input);

    checkAttributes(attributesAsChars);

    number = attributesAsChars[0] - '0'; //subtract '0' to go from ASCII of num to the actual num
    player = std::toupper(attributesAsChars[1]);
    row    = rowCharToInt(attributesAsChars[2]);
    column = attributesAsChars[3] - '0';
}

std::vector<char> Move::parseStringIntoAttributesAsChar(std::string input) const {
    //Assumes that all comments start with a '#'
    std::vector<char> attributesAsChars;
    for(char c : input) {
        if(std::isspace(c)) { continue; }
        if(c == '#') { break; }
        attributesAsChars.push_back(c);
    }
    //ex. of after this for-loop runs: 
    //attributesAsChars = {'1','X','B','2'}
    return attributesAsChars;
}


//Will always return capital A, B, or C
char Move::rowToString() const {
    if(row == 1)      { return 'A'; }
    else if(row == 2) { return 'B'; }
    else              { return 'C'; }
}
//Will always return 1, 2, or 3
int Move::rowCharToInt(char c) const {
    c = std::toupper(c);
    if(c == 'A')      { return 1; }
    else if(c == 'B') { return 2; }
    else              { return 3; }
}

void Move::checkAttributes(std::vector<char> attributesAsChars) const {
    bool success = true;
    std::string errorMessage = "";
    //Test #1 - vector size check
    if(attributesAsChars.size() != 4) { errorMessage += "Test #1 failed. Invalid vector size, expected: [4], actual: ["+std::to_string(attributesAsChars.size())+"]\n"; success = false; }
    //Test #2 - number should be an int
    if(!isdigit(attributesAsChars[0])) { errorMessage += "Test #2 failed. Invalid var type for number, expected: [int]\n"; success = false; }
    //Test #3 - column should be an int
    if(!isdigit(attributesAsChars[3])) { errorMessage += "Test #3 failed. Invalid var type for column, expected: [int]\n"; success = false; }

    //Test #4 - number should be [1, 9]
    int numberTest = attributesAsChars[0] - '0';
    if(numberTest < 1 || numberTest > 9) { errorMessage += "Test #4 failed. Invalid number range, expected: [1 to 9], actual: ["+std::to_string(numberTest)+"]\n"; success = false; }
    //Test #5 - player should be 'X' or 'O'
    char playerTest = std::toupper(attributesAsChars[1]);
    if(playerTest != 'X' && playerTest != 'O') { errorMessage += "Test #5 failed. Invalid player char, expected: ['X', 'x', 'O', or 'o'], actual: ["+std::string(1, attributesAsChars[1])+"]\n"; success = false; }
    //Test #6 - row should be 'A', 'B', or 'C'
    char rowTest = std::toupper(attributesAsChars[2]);
    if(rowTest != 'A' && rowTest != 'B' && rowTest != 'C') { errorMessage += "Test #6 failed. Invalid row char, expected: ['A', 'a', 'B', 'b', 'C', or 'c'], actual: ["+std::string(1, attributesAsChars[2])+"]\n"; success = false; }
    //Test #7 - column should be 1, 2, or 3
    int columnTest = attributesAsChars[3] - '0';
    if(columnTest != 1 && columnTest != 2 && columnTest != 3) { errorMessage += "Test #7 failed. Invalid column range, expected: [1 to 3], actual: ["+std::to_string(columnTest)+"]\n"; success = false; }

    if(!success) {
        throw ParseError(errorMessage);
    }
}

std::string Move::to_string() const {
    std::string output = std::to_string(number) + " "
                       + player + " "
                       + rowToString() 
                       + std::to_string(column);
    //output ex. 1 X B2
    return output;
}