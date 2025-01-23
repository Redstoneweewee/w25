#include <iostream>

#include "FibVec.h"

// Use this file to test your FibVec class!
// This file won't be graded - do whatever you want.
void printFibVec(FibVec fibvec);

int main() {
    FibVec fibvec;
    for(size_t i=0; i<100; i++) {
        fibvec.push(i);
    }
    printFibVec(fibvec);

    //std::string breakingPoint;
    //std::cout << "paused";
    //std::getline(std::cin, breakingPoint);
    //fibvec.remove(49);
    for(size_t i=0; i<46; i++) {
        std::cout << "removed value " << fibvec.pop() << "\n";
    }
    printFibVec(fibvec);

    return 0;
}

void printFibVec(FibVec fibvec) {
    for(size_t i=0; i<fibvec.count(); i++) {
        std::cout << i << ": " << fibvec.lookup(i) << "\n";
    }
    std::cout << "count: " << fibvec.count() << "\n";
}