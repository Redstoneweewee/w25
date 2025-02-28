#include "Counter.h"
#include "List.h"

// Counter Member Functions

Counter::Counter() {
    mList = new List();
    mCount = 0;
    mCapacity = 0;
}

Counter::~Counter() {
    if(mList != NULL) { 
        mList->~List(); 
        delete mList;
    }
}


size_t Counter::count() const {
    return mCount;
}
int Counter::total() const {
    int total = 0;
    for(auto itr = this->begin(); itr != this->end(); ++itr) {
        total += itr.value();
    }
    return total;
}

void Counter::inc(const std::string& key, int by) {
    Node* node = mList->find(key);
    if(node == NULL) { insertNewNode(key, by); }
    else { node->mValue += by; }
}
void Counter::dec(const std::string& key, int by) {
    Node* node = mList->find(key);
    if(node == NULL) { insertNewNode(key, -by); }
    else { node->mValue -= by; }
}
void Counter::del(const std::string& key) {
    if(mList->find(key)) { 
        mList->remove(key); 
        mCount--;
    }
}
int Counter::get(const std::string& key) const {
    Node* node = mList->find(key);
    if(node == NULL) { return 0; }
    else { return node->mValue; }
}
void Counter::set(const std::string& key, int count) {
    Node* node = mList->find(key);
    if(node == NULL) { insertNewNode(key, count); }
    else { node->mValue = count; }
}


Counter::Iterator Counter::begin() const {
    return Iterator(mList->head());
}
Counter::Iterator Counter::end() const {
    return Iterator(mList->head()->previous);
}

//Helper functions
void Counter::insertNewNode(const std::string& key, int value) {
    mList->insertAtTail(new Node(key, value));
    mCount++;
}