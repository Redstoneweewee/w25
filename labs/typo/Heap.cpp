#include "Heap.h"



Heap::Heap(size_t capacity) {
    mCapacity = capacity;
    mCount = 0;
    mData = new Entry[mCapacity];
}

Heap::Heap(const Heap& other) {

}

Heap::Heap(Heap&& other) {

}

Heap::~Heap() {

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

}

const Heap::Entry& Heap::top() const {
    return *mData;
}
