#include "Counter.h"
#include "List.h"

// Counter Member Functions

Counter::Counter() {
    mCount = 0;
    mCapacity = 0;
}

Counter::~Counter() {
    mList->~List();
    delete mList;
}


size_t Counter::count() const {
    return mCount;
}
int Counter::total() const {
    //finish later
    return 0;
}

void Counter::inc(const std::string& key, int by) {

}
void Counter::dec(const std::string& key, int by) {

}
void Counter::del(const std::string& key) {

}
int Counter::get(const std::string& key) const {
    return 0;
}
void Counter::set(const std::string& key, int count) {

}

Counter::Iterator Counter::begin() const {
    return Iterator(mList->head());
}
Counter::Iterator Counter::end() const {
    return Iterator(mList->head()->previous);
}