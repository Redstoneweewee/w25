#ifndef FIBVEC_H
#define FIBVEC_H

// This provides the size_t type:
#include <cstddef>

class FibVec {
    // Member Variables
    int* mFibVecPointer;
    size_t mFibNum;
    size_t mCapacity;
    size_t mCount;

    // Helper Functions
    void checkIndexOutOfRange(size_t index, size_t maxOrEqual) const;
    size_t getFibCapacity(size_t fibNum) const;
    void tryGrowCapacity(size_t newCount);
    void tryShrinkCapacity(size_t newCount);
    void modifyCapacity();

    public:
        // Constructor and Destructor
        FibVec();
        ~FibVec();

        // Member Functions
        size_t capacity() const;
        size_t count() const;
        void insert(int value, size_t index); //can throw std::out_of_range exception
        int lookup(size_t index) const; //can throw std::out_of_range exception
        int pop(); //can throw std::underflow_error exception
        void push(int value);
        int remove(size_t index); //can throw std::out_of_range exception
};

#endif
