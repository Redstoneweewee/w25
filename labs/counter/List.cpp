#include "List.h"
#include <iostream>

// List Member Functions
List::List() {
    mHead = NULL;
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

Node* List::tail() {
    if(mHead != NULL) { return mHead->previous; }
    return NULL;
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

void List::remove(Node* removeNode) {
    if(removeNode == NULL) { return; }
    if(removeNode == mHead && mHead->next == mHead) {
        mHead = NULL;
    }
    else if(removeNode == mHead) {
        mHead = mHead->next;
    }
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