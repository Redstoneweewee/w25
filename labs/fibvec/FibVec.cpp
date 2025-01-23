#include "FibVec.h"

// This provides exception types:
#include <stdexcept>


// FibVec Function Implementations

// Constructor and Destructor
FibVec::FibVec() {
    mFibVecPointer = new int[1];
    mCapacity = 1;
    mCount = 0;
}
FibVec::~FibVec() {
    delete mFibVecPointer;
}



size_t FibVec::capacity() const {
    return mCapacity;
}

size_t FibVec::count() const {
    return mCount;
}

//can throw std::out_of_range exception
void FibVec::insert(int value, size_t index) {
    //TODO - Make sure the index is in bound of count or + 1
    //     - if not, throw error
    //checkOutOfRange(index+1);

    //TODO - check if pushing will increase count past capacity
    //     - If so, resize the fibVector fibonacially

    //     - if so, move all --> to the right and add element
}

//can throw std::out_of_range exception
int FibVec::lookup(size_t index) const {
    //TODO - Make sure the index is in bound of count
    //checkOutOfRange(mCount);

    //     - if not throw
    //     - if so return the value
    return 1;
}

//can throw std::underflow_error exception
int FibVec::pop() {
    //TODO - make sure the count > 0
    //     - if not throw error
    //     - if so just decrease count by 1
    return 1;
}

void FibVec::push(int value) {
    //TODO - check if pushing will increase count past capacity
    //     - If so, resize the fibVector fibonacially
    //Otherwise, just add value
}

//can throw std::out_of_range exception
int FibVec::remove(size_t index) {
    //TODO - Make sure the index is in bound of count
    //checkOutOfRange(mCount);

    //     - if not, throw error
    //     - if so, must <--- push all after to the left & decrease count by 1
    return 1;
}
