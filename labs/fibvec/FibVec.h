#ifndef FIBVEC_H
#define FIBVEC_H

// This provides the size_t type:
#include <cstddef>
#include <vector>

class FibVec {
    // Member Variables
    std::vector<int>* vectorPointer;
    // Helper Functions

   public:
    // Constructor and Destructor
    FibVec();
    ~FibVec();

    // Member Functions
    unsigned int capacity();
    unsigned int count();
    void insert(int value, unsigned int index); //can throw std::out_of_range exception
    int lookup(unsigned int index); //can throw std::out_of_range exception
    int pop(); //can throw std::underflow_error exception
    void push(int value);
    int remove(unsigned int index); //can throw std::out_of_range exception
};

#endif
