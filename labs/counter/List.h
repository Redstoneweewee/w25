#ifndef LIST_H
#define LIST_H

#include <string>

// TODO...

struct Node {
    std::string mKey;
    int mValue;
    Node* next;
    Node* previous;

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
};

class List {
  private:
    Node* mHead;
    size_t mCount;

  public:
    List();
    ~List();

    Node* head();
    size_t count();
    void insertAtTail(Node* node);
    /**
     * Returns a pointer to the node with key. If not found, returns NULL.
     */
    Node* find(std::string key);
    void remove(std::string key);
    void printAll();
};

#endif
