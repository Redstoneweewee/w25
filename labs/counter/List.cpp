#include "List.h"
#include <iostream>

// List Member Functions
List::List() {
    mHead = NULL;
    mCount = 0;
}

List::~List() {
    if(mHead == NULL) { return; }
    mHead->previous->next = NULL;
    Node* deleteNode = mHead;
    Node* nextNode = mHead->next;
    delete mHead;
    while(nextNode != NULL) {
        deleteNode = nextNode;
        nextNode = nextNode->next;
        delete deleteNode;
    }
}

Node* List::head() {
    return mHead;
}

size_t List::count() {
    return mCount;
}

void List::insertAtTail(Node* node) {
    if(mHead != NULL) {
        Node* oldTail = mHead->previous;
        mHead->previous->next = node;
        mHead->previous = node;
        node->next = mHead;
        node->previous = oldTail;
    }
    else {
        mHead = node;
        node->next = node;
        node->previous = node;
    }
}

Node* List::find(std::string key) {
    if(mHead == NULL) { return NULL; }
    if(mHead->mKey == key) { return mHead; }
    Node* nextNode = mHead->next;
    while(nextNode != mHead) {
        if(nextNode->mKey == key) { return nextNode; }
        nextNode = nextNode->next;
    }
    return NULL;
}

void List::remove(std::string key) {
    Node* removeNode = find(key);
    if(removeNode == NULL) { return; }

    removeNode->previous->next = removeNode->next;
    removeNode->next->previous = removeNode->previous;
    delete removeNode;
}


void List::printAll() {
    if(mHead == NULL) { return; }
    size_t i = 0;
    std::cout << "obj at " << i << ": key=" << mHead->mKey << ", value=" << mHead->mValue << "\n";
    Node* nextNode = mHead->next;
    while(nextNode != mHead) {
        i++;
        std::cout << "obj at " << i << ": key=" << nextNode->mKey << ", value=" << nextNode->mValue << "\n";
        nextNode = nextNode->next;
    }
}