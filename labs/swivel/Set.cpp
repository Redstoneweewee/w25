#include "Set.h"

bool isNullNode(Node* node);


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
    if(isNullNode(mRoot)) {
        removeCount += mRoot->removeTree();
    }
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
    }
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


bool isNullNode(Node* node) {
    if(node == NULL) { return true; }
    return false;
}