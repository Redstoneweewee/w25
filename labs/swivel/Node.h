#ifndef NODE_H
#define NODE_H

#include <string>

// Use this file to declare your Node type.
// Implement Node member functions and helper functions in Node.cpp.
class Node {
   public:
    std::string data;
    Node* left;
    Node* right;
    Node();
    Node(std::string data);
    Node(std::string data, Node* left, Node* right);

    size_t removeTree();
    bool   treeContains(std::string value) const;
    size_t insertIntoTree(std::string value);
};

#endif
