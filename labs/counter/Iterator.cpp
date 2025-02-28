#include "Counter.h"

Counter::Iterator::Iterator(Node* startNode) {
    mNode = startNode;
}

// Counter::Iterator Member Functions
const std::string& Counter::Iterator::key() const {
    return mNode->mKey;
}
int Counter::Iterator::value() const {
    return mNode->mValue;
}

void Counter::Iterator::operator ++ () {
    mNode = mNode->next;
}
bool Counter::Iterator::operator == (const Iterator& other) const {
    return mNode == other.mNode;
}
bool Counter::Iterator::operator != (const Iterator& other) const {
    return mNode != other.mNode;
}