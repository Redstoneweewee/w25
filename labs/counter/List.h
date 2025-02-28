#ifndef LIST_H
#define LIST_H

#include <string>

// TODO...
struct Node {
    long long int mHashNum = -1;
    std::string mKey;
    int mValue;
    Node* next;
    Node* previous;

    ~Node() {
        next = NULL;
        previous = NULL;
    }

    Node() {
        next = NULL;
        previous = NULL;
    }
    Node(std::string key, int value) {
        mKey = key;
        mValue = value;
        next = NULL;
        previous = NULL;
    }
    void setHashNum(size_t hashNum) {
        mHashNum = hashNum;
    }
};

class List {
  private:
    Node* mHead;

  public:
    List();
    ~List();

    Node* head();
    Node* tail();
    void insertAtTail(Node* node);
    /**
     * Returns a pointer to the node with key. If not found, returns NULL.
     */
    Node* find(std::string key);
    void remove(Node* removeNode);
    void printAll();
};

#endif
