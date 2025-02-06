#include "Set.h"

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

}


size_t Set::clear() {
    return 0;
}

bool Set::contains(const std::string& value) const {
    return false;
}

size_t Set::count() const {
    return 0;
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
