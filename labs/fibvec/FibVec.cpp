#include "FibVec.h"

// This provides exception types:
#include <stdexcept>


// FibVec Function Implementations

// Constructor and Destructor
FibVec::FibVec() {
    fibVecPointer = new int[1];
}
FibVec::~FibVec() {
    delete fibVecPointer;
}

// Member Functions
unsigned long int FibVec::capacity() const {
    return 0;
}

unsigned long int FibVec::count() const {
    return 0;
}

//can throw std::out_of_range exception
void FibVec::insert(int value, unsigned long int index) {

}

//can throw std::out_of_range exception
int FibVec::lookup(unsigned long int index) const {
    return 1;
}

//can throw std::underflow_error exception
int FibVec::pop() {
    return 1;
}

void FibVec::push(int value) {

}

//can throw std::out_of_range exception
int FibVec::remove(unsigned long int index) {
    return 1;
}
