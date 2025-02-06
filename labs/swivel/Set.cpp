#include "Set.h"

#include <iostream>

bool isNullNode(Node* node);


Set::Set() {
    mRoot = NULL;
    mCount = 0;
}

Set::Set(const Set& other) {
    if(other.mRoot == NULL) {
        Set();
    }
    else {
        mRoot = other.mRoot->createCopy();
        mCount = other.mCount;
    }
}

Set::Set(Set&& other) {
    mRoot = other.mRoot;
    mCount = other.mCount;

    other.mRoot = NULL;
    other.~Set();
}

Set::~Set() {
    clear();
}


size_t Set::clear() {
    size_t removeCount = 0;
    if(!isNullNode(mRoot)) {
        removeCount += mRoot->removeTree();
        mRoot = NULL;
    }
    mCount = 0;
    return removeCount;
}

bool Set::contains(const std::string& value) const {
    if(isNullNode(mRoot)) { return false; }
    return mRoot->treeContains(value);
}

size_t Set::count() const {
    return mCount;
}

void Set::debug() {

}

size_t Set::insert(const std::string& value) {
    if(isNullNode(mRoot)) {
        mRoot = new Node(value);
        mCount++;
        return 1;
    }
    size_t insertSuccess = mRoot->insertIntoTree(value);
    if(insertSuccess == 1) { mCount++; }
    return insertSuccess;
}

void Set::print() const {
    if(isNullNode(mRoot)) {
        std::cout << "-\n";
    }
    else {
        std::cout << mRoot->treeToString() << "\n";
    }
}

size_t Set::remove(const std::string& value) {
    if(!swivel(value)) { 
        return 0;
    }
    Node* deleteNode = mRoot;
    if(mRoot->left == NULL && mRoot->right == NULL) {
        delete deleteNode;
        mRoot = NULL;
    }
    else if(mRoot->left == NULL) {
        mRoot = mRoot->right;
        delete deleteNode;
    }
    else if(mRoot->right == NULL) {
        mRoot = mRoot->left;
        delete deleteNode;
    }
    else {
        std::string swivelValue = mRoot->getRightSmallestValue();
        mRoot->right->swivelTree(swivelValue);
        mRoot->right->left = mRoot->left;
        mRoot = mRoot->right;
        delete deleteNode;
    }
    mCount -= 1;
    return 1;
}

bool Set::swivel(const std::string& value) {
    if(isNullNode(mRoot)) { return false; }
    if(contains(value)) { 
        mRoot = mRoot->swivelTree(value);
        return true;
    }
    return false;
}


bool isNullNode(Node* node) {
    if(node == NULL) { return true; }
    return false;
}