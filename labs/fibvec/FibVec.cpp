#include "FibVec.h"

// This provides exception types:
#include <stdexcept>
#include <iostream>


// FibVec Function Implementations

// Constructor and Destructor
FibVec::FibVec() {
    mFibVecPointer = new int[1];
    mFibNum = 2;
    mCapacity = 1;
    mCount = 0;
}
FibVec::~FibVec() {
    delete [] mFibVecPointer;
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
    checkIndexOutOfRange(index, mCount);

    //TODO - check if pushing will increase count past capacity
    //     - If so, resize the fibVector fibonacially
    tryGrowCapacity(mCount+1);

    //     - if so, move all --> to the right and add element
    for(size_t i=mCount-1; i>=index; i--) {
        mFibVecPointer[i+1] = mFibVecPointer[i];
    }
    mFibVecPointer[index] = value;
    mCount++;
}

//can throw std::out_of_range exception
int FibVec::lookup(size_t index) const {
    //TODO - Make sure the index is in bound of count
    //     - if not throw
    checkIndexOutOfRange(index, mCount-1);
    
    //     - if so return the value
    return mFibVecPointer[index];
}

//can throw std::underflow_error exception
int FibVec::pop() {
    //TODO - make sure the count > 0
    //     - if not throw error
    if(mCount == 0) {
        throw std::underflow_error("Tried to pop an empty FibVec.");
    }
    //     - if so just use remove() on the last element
    return remove(mCount-1);
}

void FibVec::push(int value) {
    //TODO - check if pushing will increase count past capacity
    //     - If so, resize the fibVector fibonacially
    tryGrowCapacity(mCount+1);
    //Otherwise, just add value
    mFibVecPointer[mCount] = value;
    mCount++;
}

//can throw std::out_of_range exception
int FibVec::remove(size_t index) {
    //TODO - Make sure the index is in bound of count
    //     - if not, throw error
    checkIndexOutOfRange(index, mCount-1);

    //     - if so, must <--- push all after to the left & decrease count by 1
    int removedValue = mFibVecPointer[index];
    mCount--;
    for(size_t i=index; i<mCount; i++) {
        mFibVecPointer[i] = mFibVecPointer[i+1];
    }

    //TODO - check if removing will decrease count to < f(n-2)
    //     - If so, resize the fibVector to f(n-1)
    tryShrinkCapacity(mCount);
    return removedValue;
}


void FibVec::checkIndexOutOfRange(size_t index, size_t max) const {
    if(index > max) {
        std::string errorMessage = "index: "+std::to_string(index)+" is greater than max: "+std::to_string(max);
        throw std::out_of_range(errorMessage);
    }
}

size_t FibVec::getFibCapacity(size_t fibNum) const {
    if(fibNum <= 2) {
        return 1;
    }
    else {
        return getFibCapacity(fibNum-2) + getFibCapacity(fibNum-1);
    }
}

void FibVec::tryGrowCapacity(size_t newCount) {
    if(newCount <= mCapacity) { return; }

    mFibNum++;
    modifyCapacity();
    //std::cout << "new capacity: " << mCapacity << "\n";
}

void FibVec::tryShrinkCapacity(size_t newCount) {
    if(newCount >= getFibCapacity(mFibNum-2)) { return; }
    mFibNum--;
    modifyCapacity();
    //std::cout << "new capacity: " << mCapacity << "\n";
}

void FibVec::modifyCapacity() {
    mCapacity = getFibCapacity(mFibNum);
    int* temp = mFibVecPointer;
    mFibVecPointer = new int[mCapacity];
    for(size_t i=0; i<mCount; i++) {
        mFibVecPointer[i] = temp[i];
        //std::cout << "added back: " << i << ", " << mFibVecPointer[i] << "\n";
    }
    delete [] temp;
}