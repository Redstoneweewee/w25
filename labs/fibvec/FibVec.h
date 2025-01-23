#ifndef FIBVEC_H
#define FIBVEC_H

// This provides the size_t type:
#include <cstddef>

class FibVec {
    // Member Variables
    int* fibVecPointer;
    // Helper Functions

   public:
    // Constructor and Destructor
    FibVec();
    ~FibVec();

    // Member Functions
    unsigned long int capacity() const;
    unsigned long int count() const ;
    void insert(int value, unsigned long int index); //can throw std::out_of_range exception
    int lookup(unsigned long int index) const; //can throw std::out_of_range exception
    int pop(); //can throw std::underflow_error exception
    void push(int value);
    int remove(unsigned long int index); //can throw std::out_of_range exception
};

#endif
