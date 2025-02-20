#include "Heap.h"
#include <stdexcept>


Heap::Heap(size_t capacity) {
    mCapacity = capacity;
    mCount = 0;
    mData = new Entry[mCapacity];
}

Heap::Heap(const Heap& other) {
    mCapacity = other.mCapacity;
    mCount = other.mCount;
    mData = new Entry[mCapacity];
    for(size_t i=0; i<mCapacity; i++) {
        mData[i] = other.mData[i];
    }
}

Heap::Heap(Heap&& other) {
    mData = other.mData;
    mCapacity = other.mCapacity;
    mCount = other.mCount;
    other.mData = NULL;
}

Heap::~Heap() {
    delete [] mData;
}



size_t Heap::capacity() const {
    return mCapacity;
}

size_t Heap::count() const {
    return mCount;
}

const Heap::Entry& Heap::lookup(size_t index) const {
    return *mData;
}

Heap::Entry Heap::pop() {
    return *mData;
}

Heap::Entry Heap::pushpop(const std::string& value, float score) {
    return *mData;
}

void Heap::push(const std::string& value, float score) {
    mCount++;
}

const Heap::Entry& Heap::top() const {
    return *mData;
}
