#include "List.h"

List::List() {
    head = new Node;
    mCount = 0;
}

List::List(const List& other) {
    //TODO - make a new node from each node in other
}

List::~List() {
    //TODO - go through to the end of the LL and remove all one by one
}

size_t List::count() const {
    return mCount;
}