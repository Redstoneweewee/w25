#include "List.h"
#include <iostream>
#include <stdexcept>


//Helper functions
size_t getInsertIndexFromAsciiSort(const Node* node, const std::string& value);
bool isStringAsciiLesserOrEqual(const std::string& str1, const std::string& str2);


List::List() {
    head = NULL;
}

List::List(const List& other) {
    //TODO - make a new node from each node in other
    head = NULL;
    Node* otherNextNode = other.head;
    if(otherNextNode != NULL) {
        head = new Node();
        head->data = otherNextNode->data;
        //head->next = NULL;
        otherNextNode = otherNextNode->next;
    }

    Node* previousNode = head;
    while(otherNextNode != NULL) {
        Node* newNode = new Node();
        previousNode->next = newNode;
        newNode->data = otherNextNode->data;
        previousNode = newNode;
        otherNextNode = otherNextNode->next;
    }
}

List::~List() {
    //TODO - go through to the end of the LL and remove all one by one
    Node* nextMode = head;
    while(nextMode != NULL) {
        Node* deleteNode = nextMode;
        nextMode = nextMode->next;
        delete deleteNode;
    }
}

size_t List::count() const {
    size_t count = 0;
    Node* nextMode = head;
    while(nextMode != NULL) {
        count++;
        nextMode = nextMode->next;
    }
    return count;
}

void List::insert(const std::string& value) {
    //TODO - append the value in the correct index after sort detection
    size_t insertIndex = getInsertIndexFromAsciiSort(head, value);
    if(insertIndex == 0) {
        Node* afterNode = head;
        head = new Node;
        head->data = value;
        head->next = afterNode;
    }
    else {
        Node* beforeNode = head;
        for(size_t i=0; i<insertIndex-1; i++) {
            beforeNode = beforeNode->next;
        }
        Node* afterNode = beforeNode->next;

        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = afterNode;
        beforeNode->next = newNode;
    }
}

const std::string& List::lookup(size_t index) const {
    size_t current = 0;
    Node* nextNode = head;
    while(nextNode != NULL) {
        if(index == current) {
            return nextNode->data;
        }
        nextNode = nextNode->next;
        current++;
    }
    throw std::out_of_range("Out of bounds. Tried to lookup at index "+std::to_string(index)+" but the Linked List's size is "+std::to_string(count()));
    exit(1);
}

void List::print(bool reverse) const {
    Node* nextNode = head;

    if(!reverse) {
        std::string output = "[";
        while(nextNode != NULL) {
            output += (nextNode->data + ", ");
            nextNode = nextNode->next;
        }
        output = output.substr(0, output.size()-2);
        output += "]\n";
        std::cout << output;
    }
    else {
        std::string output = "";
        while(nextNode != NULL) {
            output = nextNode->data + ", " + output;
            nextNode = nextNode->next;
        }
        output = output.substr(0, output.size()-2);
        output = "[" + output + "]\n";
        std::cout << output;
    }
}

std::string List::remove(size_t index) {
    if(index == 0) {
        if(count() == 0) {
            throw std::out_of_range("Tried to remove from an empty Linked List.");
            exit(1);
        }
        Node* removeNode = head;
        head = head->next;
        std::string removedString = removeNode->data;
        delete removeNode;
        return removedString;
    }

    size_t current = 1;
    Node* beforeNode = head;
    Node* removeNode = beforeNode->next;
    while(removeNode != NULL) {
        if(index == current) {
            beforeNode->next = removeNode->next;
            std::string removedString = removeNode->data;
            delete removeNode;
            return removedString;
        }
        beforeNode = removeNode;
        removeNode = removeNode->next;
        current++;
    }
    throw std::out_of_range("Out of bounds. Tried to remove at index "+std::to_string(index)+" but the Linked List's size is "+std::to_string(count()));
    exit(1);
}

size_t List::remove(const std::string& value) {
    size_t count = 0;
    if(head == NULL) {
        return count; 
    }
    
    while(head != NULL && head->data == value) {
        Node* removeNode = head;
        head = head->next;
        delete removeNode;
        count++;
    }
    
    Node* beforeNode = head;
    Node* nextNode = head->next;
    while(nextNode != NULL) {
        if(nextNode->data == value) {
            Node* removeNode = nextNode;
            beforeNode->next = removeNode->next;
            delete removeNode;
            count++;
            nextNode = beforeNode->next;
            continue;
        }
        beforeNode = nextNode;
        nextNode = nextNode->next;
    }
    return count;
}


//Helper functions
size_t getInsertIndexFromAsciiSort(const Node* node, const std::string& value) {
    //TODO - return the index of where the value should be stored
    size_t index = 0;
    while(node != NULL && isStringAsciiLesserOrEqual(node->data, value)) {
        node = node->next;
        index++;
    }
    return index;
}


bool isStringAsciiLesserOrEqual(const std::string& str1, const std::string& str2) {
    if(str1.size() == 0) { return true; }
    if(str2.size() == 0) { return false; }

    size_t stringIndex = 0;
    //compares the two strings. If the characters are equal, then move onto the next character to compare.
    //If we have reached the end of either string, then stop the loop
    while(str1.size() > stringIndex && str2.size() > stringIndex && str1[stringIndex] == str2[stringIndex]) {
        stringIndex++;
    }
    //If we have reached the end of either string and their values are the same,
    //returns true if str2 is longer or equal, false otherwise
    if(str1[stringIndex] == str2[stringIndex]) {
        if(str1.size() <= str2.size()) { return true; }
        return false;
    }
    else if(str1[stringIndex] < str2[stringIndex]) {
        return true;
    }
    return false;
}