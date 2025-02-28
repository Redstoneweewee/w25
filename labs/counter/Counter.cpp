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

void Counter::inc(const std::string& key, int by) {
    Node* node = mIndex->find(key);
    if(node == NULL) { insertNewNode(key, by); }
    else { node->mValue += by; }
}
void Counter::dec(const std::string& key, int by) {
    Node* node = mIndex->find(key);
    if(node == NULL) { insertNewNode(key, -by); }
    else { node->mValue -= by; }
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
    if(node == NULL) { return 0; }
    else { return node->mValue; }
}
void Counter::set(const std::string& key, int count) {
    Node* node = mIndex->find(key);
    if(node == NULL) { insertNewNode(key, count); }
    else { node->mValue = count; }
}


Counter::Iterator Counter::begin() const {
    return Iterator(mList->head());
}
Counter::Iterator Counter::end() const {
    return Iterator(NULL);
}

//Helper functions
void Counter::insertNewNode(const std::string& key, int value) {
    mList->insertAtTail(new Node(key, value));
    mIndex->insert(mList->tail());
}