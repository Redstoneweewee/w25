#include "Heap.h"
#include <stdexcept>

enum class PDir {
    UP,
    DOWN
};
void percolate(Heap::Entry* array, size_t size, size_t index, PDir direction);
void swap(Heap::Entry& entry1, Heap::Entry& entry2);


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
    mCapacity = other.mCapacity;
    mCount = other.mCount;
    mData = other.mData;
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
    if(index >= mCount) {
        throw std::out_of_range("Tried to lookup an invalid index. The size of the heap is: "
                                +std::to_string(mCount)
                                +" while the lookup index is: "
                                +std::to_string(index));
    }
    return mData[index];
}

Heap::Entry Heap::pop() {
    if(mCount == 0) {
        throw std::underflow_error("Tried to pop from an empty heap.");
    }

    Entry poppedEntry = *mData;
    mData[0] = mData[mCount-1];
    percolate(mData, mCount, 0, PDir::DOWN);
    mCount--;
    return poppedEntry;
}

Heap::Entry Heap::pushpop(const std::string& value, float score) {
    if(mCount == 0) {
        throw std::underflow_error("Tried to pop from an empty heap.");
    }

    Entry poppedEntry = *mData;
    mData[0] = Entry{value, score};
    percolate(mData, mCount, 0, PDir::DOWN);
    return poppedEntry;
}

void Heap::push(const std::string& value, float score) {
    if(mCount == mCapacity) {
        throw std::overflow_error("Tried to push into a full heap.");
    }

    mData[mCount] = Entry{value, score};
    percolate(mData, mCount, mCount, PDir::UP);
    mCount++;
}

const Heap::Entry& Heap::top() const {
    return *mData;
}


void percolate(Heap::Entry* array, size_t size, size_t index, PDir direction) {
    if(direction == PDir::UP) {
        while(index != 0 && array[index].score > array[(index-1)/2].score) {
            swap(array[index], array[(index-1)/2]);
            index = (index-1)/2;
        }
    }
    else {
        while(true) { //hack
            if(index*2+1 < size && array[index].score < array[index*2+1].score) {
                swap(array[index], array[index*2+1]);
                index = index*2+1;
            }
            else if(index*2+2 < size && array[index].score < array[index*2+2].score) {
                swap(array[index], array[index*2+2]);
                index = index*2+2;
            }
            else { return; }
        }
    }
}

void swap(Heap::Entry& entry1, Heap::Entry& entry2) {
    Heap::Entry temp = entry1;
    entry1 = entry2;
    entry2 = temp;
}