#ifndef COUNTER_H
#define COUNTER_H

#include "List.h"
#include <cstddef>
#include <string>

// This is the Counter class you need to implement.
// It includes the Counter::Iterator as a nested class.

class Counter {
public:
  class Iterator {
    // Member Variables
    Node* mNode;

  public:
    Iterator(Node* startNode);
    const std::string& key() const;
    int value() const;

    void operator ++ ();
    bool operator == (const Iterator& other) const;
    bool operator != (const Iterator& other) const;
  };

private:
  // Member Variables
    List* mList;
    size_t mCount;
    size_t mCapacity;

private:
  // Helper Functions
  void insertNewNode(const std::string& key, int value);

public:
  Counter();
  ~Counter();

  size_t count() const;
  int    total() const;

  void inc(const std::string& key, int by = 1);
  void dec(const std::string& key, int by = 1);
  void del(const std::string& key);
  int  get(const std::string& key) const;
  void set(const std::string& key, int count);

  Iterator begin() const;
  Iterator end() const;
};

#endif
