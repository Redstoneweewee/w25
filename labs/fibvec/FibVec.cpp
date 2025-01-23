#include "FibVec.h"

// This provides exception types:
#include <stdexcept>


// FibVec Function Implementations

// Constructor and Destructor
FibVec::FibVec() {
    vectorPointer = new std::vector<int>(1);
}
FibVec::~FibVec() {
    delete vectorPointer;
}

// Member Functions
unsigned int FibVec::capacity() {
    return 0;
}

unsigned int FibVec::count() {
    return 0;
}

//can throw std::out_of_range exception
void FibVec::insert(int value, unsigned int index) {

}

//can throw std::out_of_range exception
int FibVec::lookup(unsigned int index) {
    return 1;
}

//can throw std::underflow_error exception
int FibVec::pop() {
    return 1;
}

void FibVec::push(int value) {

}

//can throw std::out_of_range exception
int FibVec::remove(unsigned int index) {
    return 1;
}
