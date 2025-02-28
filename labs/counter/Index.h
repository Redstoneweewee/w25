#ifndef INDEX_H
#define INDEX_H

#include "List.h"

const size_t initialSize = 65536; //2^16
// TODO...


struct Item {
    Node* node = NULL;
    bool isDirty = false;
};

class Index {
  private:
    Item* mTable; 
    size_t mCount;
    size_t mCapacity;

    size_t hash(const std::string& string) const;
    size_t probeEmpty(size_t index, size_t n = 1) const;
    long long int probeKey(size_t index, const std::string& key, size_t n = 1) const;
    void tryResizeUp();

  public:
    Index();
    ~Index();

    size_t count() const;
    void insert(Node* node);
    Node* find(const std::string& key) const;
    int lookup(const std::string& key) const;
    void remove(const std::string& key); 
};

#endif
