#include "Set.h"

bool checkNullNode(Node* node);


Set::Set() {
    mRoot = NULL;
    mCount = 0;
}

Set::Set(const Set& other) {

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
    if(checkNullNode(mRoot)) {
        removeCount += mRoot->removeTree();
    }
    return removeCount;
}

bool Set::contains(const std::string& value) const {
    if(checkNullNode(mRoot)) { return false; }
    return mRoot->treeContains(value);
}

size_t Set::count() const {
    return mCount;
}

void Set::debug() {

}

size_t Set::insert(const std::string& value) {
    
    return 0;
}

void Set::print() const {

}

size_t Set::remove(const std::string& value) {
    return 0;
}

bool Set::swivel(const std::string& value) {
    return false;
}


bool checkNullNode(Node* node) {
    if(node == NULL) { return true; }
    return false;
}