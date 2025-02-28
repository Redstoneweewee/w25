#include "Index.h"
#include <iostream>

// Index Member Functions
Index::Index() {
    mTable = new Item[initialSize]{NULL};
    mCount = 0;
    mCapacity = initialSize;
}

Index::~Index() {
    delete [] mTable;
}

size_t Index::count() const {
    return mCount;
}

size_t Index::hash(const std::string& string) const {
    size_t output = 0;
    for(char c : string) {
        output += c;
    }
    return output;
}

void Index::insert(Node* node) {
    if(node->mHashNum == -1) {
        node->mHashNum = hash(node->mKey);
        mTable[probeEmpty(node->mHashNum % mCapacity)].isDirty = true;
        mCount++;
    }
    mTable[probeEmpty(node->mHashNum % mCapacity)].node = node;
    tryResizeUp();
}


size_t Index::probeEmpty(size_t index, size_t n) const {
    if(index >= mCapacity) {
        index = index % mCapacity;
    }
    if(mTable[index].node == NULL) {
        return index;
    }
    return probeEmpty(index+n, n+1);
}

long long int Index::probeKey(size_t index, const std::string& key, size_t n) const {
    
    if(index >= mCapacity) {
        index = index % mCapacity;
    }

    if(mTable[index].node != NULL) {
        if(mTable[index].node->mKey == key) {
            return index;
        }
        else {
            return probeKey(index+n, key, n+1);
        }
    }
    else if(mTable[index].isDirty) {
        return probeKey(index+n, key, n+1);
    }
    return -1;
}

void Index::tryResizeUp() {
    if(mCount <= mCapacity/2) { return; }
    mCapacity *= 2;
    Item* oldTable = mTable;
    mTable = new Item[mCapacity]{NULL};

    for(size_t i=0; i<mCapacity/2; i++) {
        if(oldTable[i].node == NULL) { continue; }
        const size_t newIndex = probeEmpty(oldTable[i].node->mHashNum % mCapacity);
        mTable[newIndex] = oldTable[i];
    }
    delete [] oldTable;
}

Node* Index::find(const std::string& key) const {
    const long long int index = probeKey(hash(key) % mCapacity, key);
    if(index == -1) {
        return NULL;
    }
    return mTable[index].node;
}

int Index::lookup(const std::string& key) const {
    const long long int index = probeKey(hash(key) % mCapacity, key);
    if(index == -1) {
        return 0;
    }
    return mTable[index].node->mValue;
}

void Index::remove(const std::string& key) {
    const long long int index = probeKey(hash(key) % mCapacity, key);
    if(index == -1) { return; }
    mTable[index].node = NULL;
    mCount--;
}