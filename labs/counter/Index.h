#ifndef INDEX_H
#define INDEX_H

#include "List.h"

// TODO...
class Index {
  private:
    Node* mArray[20];
    size_t mCount;
    size_t mCapacity;

  public:
    Index();
    Index(size_t size);

    size_t hash(std::string key);
    size_t count();
    size_t capacity();
};

#endif
