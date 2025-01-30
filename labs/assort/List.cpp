#include "List.h"

List::List() {
    head = new Node;
}

List::List(const List& other) {
    //TODO - make a new node from each node in other
}

List::~List() {
    //TODO - go through to the end of the LL and remove all one by one
}

size_t List::count() const {
    return 0;
}

void List::insert(const std::string& value) {

}

const std::string& List::lookup(size_t index) const {
    return head->data;
}

void List::print(bool reverse) const {

}

std::string List::remove(size_t index) {
    return "";
}

size_t List::remove(const std::string& value) {
    return 0;
}