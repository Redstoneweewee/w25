#include "Counter.h"
#include "List.h"
#include <iostream>

// Counter Member Functions

Counter::Counter() {
    mList = new List();
    mIndex = new Index();
}

Counter::~Counter() {
    delete mList;
    delete mIndex;
}


size_t Counter::count() const {
    return mIndex->count();
}
int Counter::total() const {
    int total = 0;
    if(mList->head() == NULL) { return total; } 
    for(auto itr = this->begin(); itr != this->end(); ++itr) {
        total += itr.value();
    }
    return total;
}

void Counter::inc(const std::string& key, const int by) {
    Node* node = mIndex->find(key);
    if(node != NULL) { node->mValue += by; }
    else { insertNewNode(key, by); }
}
void Counter::dec(const std::string& key, const int by) {
    Node* node = mIndex->find(key);
    if(node != NULL) { node->mValue -= by; }
    else { insertNewNode(key, -by); }
}
void Counter::del(const std::string& key) {
    Node* node = mIndex->find(key);
    if(node != NULL) { 
        mIndex->remove(key);
        mList->remove(node);
    }
}
int Counter::get(const std::string& key) const {
    Node* node = mIndex->find(key);
    if(node != NULL) { return node->mValue; }
    else { return 0; }
}
void Counter::set(const std::string& key, const int count) const {
    Node* node = mIndex->find(key);
    if(node != NULL) { node->mValue = count; }
    else { insertNewNode(key, count); }
}


Counter::Iterator Counter::begin() const {
    return Iterator(mList->head());
}
Counter::Iterator Counter::end() const {
    return Iterator(NULL);
}

//Helper functions
void Counter::insertNewNode(const std::string& key, const int value) const {
    Node* node = new Node(key, value);
    mList->insertAtTail(node);
    mIndex->insert(node);
}