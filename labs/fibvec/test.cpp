#include <iostream>

#include "FibVec.h"

// Use this file to test your FibVec class!
// This file won't be graded - do whatever you want.
void printFibVec(FibVec& fibvec);

int main() {
    FibVec fibvec;

    printFibVec(fibvec);
    fibvec.push(-120);
    printFibVec(fibvec);
    fibvec.push(-120);
    printFibVec(fibvec);
    fibvec.pop();
    printFibVec(fibvec);
    fibvec.pop();
    printFibVec(fibvec);
    std::cout << "\n\n";
    fibvec.push(-120);
    printFibVec(fibvec);
    fibvec.push(-120);
    printFibVec(fibvec);
    fibvec.pop();
    printFibVec(fibvec);
    fibvec.pop();
    printFibVec(fibvec);

    return 0;
}

void printFibVec(FibVec& fibvec) {
    for(size_t i=0; i<fibvec.count(); i++) {
        std::cout << i << ": " << fibvec.lookup(i) << "\n";
    }
    std::cout << "count: " << fibvec.count() << "\n";
    std::cout << "capacity: " << fibvec.capacity() << "\n";
}