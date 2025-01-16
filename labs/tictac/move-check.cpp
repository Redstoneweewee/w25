#include <iostream>

#include "Errors.h"
#include "Move.h"

int main(int argc, char **argv) {
    std::cout << "Parse error.\n";
    return 1;
    /*
    bool verbose = false;

    if (argc == 2 && std::string(argv[1]) == "-v") {
        verbose = true;
    }

    if (verbose) {
        std::cout << "> ";
    }

    std::string line;
    std::getline(std::cin, line);

    try {
        Move move(line);
        std::cout << move.to_string() << '\n';
        return 0;
    } 
    catch (const ParseError &e) {
        if (verbose) {
            std::cout << "Parse error: \n" << e.what() << '\n';
        } else {
            std::cout << "Parse error.\n";
        }

        return 1;
    }
    */
}
