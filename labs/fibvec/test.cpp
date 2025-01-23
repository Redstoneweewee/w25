#include <iostream>

#include "FibVec.h"

// Use this file to test your FibVec class!
// This file won't be graded - do whatever you want.
void printFibVec(FibVec& fibvec);

int main() {
    FibVec fibvec;
    //for(size_t i=0; i<10; i++) {
    //    fibvec.push(i);
    //}
    //fibvec.insert(10, 0);
    //printFibVec(fibvec);
    //std::cout << "count: " << fibvec.count() << "\n";
    //std::cout << "value: " << fibvec.lookup(0) << "\n";

    
    for(size_t i=0; i<100; i++) {
        fibvec.push(i);
    }

    //for(size_t i=0; i<3; i++) {
    //    fibvec.remove(2);
    //}
    printFibVec(fibvec);
    //printFibVec(fibvec);
    //std::cout << "what" << "\n";
    //std::string breakingPoint;
    //std::cout << "paused";
    //std::getline(std::cin, breakingPoint);
    //fibvec.remove(49);
    for(size_t i=0; i<67; i++) {
        fibvec.remove(0);
        //std::cout << "removed value " << fibvec.pop() << "\n";
    }
    printFibVec(fibvec);
    std::cout << "removed at 32: " << fibvec.remove(32) << "\n";
    std::cout << "removed at 32: " << fibvec.remove(32) << "\n";
    //printFibVec(fibvec);
    
    return 0;
}

void printFibVec(FibVec& fibvec) {
    for(size_t i=0; i<fibvec.count(); i++) {
        std::cout << i << ": " << fibvec.lookup(i) << "\n";
    }
    std::cout << "count: " << fibvec.count() << "\n";
    std::cout << "capacity: " << fibvec.capacity() << "\n";
}