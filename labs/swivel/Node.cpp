#include "Node.h"

Node::Node(std::string data) {
    this->data = data;
    left  = NULL;
    right = NULL;
}

Node::Node(std::string data, Node* left, Node* right) {
    this->data  = data;
    this->left  = NULL;
    this->right = NULL;
}


size_t Node::removeTree() {
    size_t removeCount = 1;
    if(this->left != NULL) {
        removeCount += this->left->removeTree();
    }
    if(this->right != NULL) {
        removeCount += this->right->removeTree();
    }
    delete this;
    return removeCount;
}

bool Node::treeContains(std::string value) const {
    if(value.compare(this->data) < 0) {
        if(this->left != NULL) {
            return this->left->treeContains(value);
        }
        else { return false; }
    }
    else if(value.compare(this->data) > 0) {
        if(this->right != NULL) {
            return this->right->treeContains(value);
        }
        else { return false; }
    }
    else { return true; }
}

size_t Node::insertIntoTree(std::string value) {
    if(value.compare(this->data) < 0) {
        if(this->left != NULL) {
            return this->left->insertIntoTree(value);
        }
        else {
            this->left = new Node(value);
            return 1;
        }
    }
    else if(value.compare(this->data) > 0) {
        if(this->right != NULL) {
            return this->right->insertIntoTree(value);
        }
        else {
            this->right = new Node(value);
            return 1;
        }
    }
    else {
        return 0;
    }
}

std::string Node::treeToString() {
    std::string leftString;
    std::string rightString;

    if(this->left != NULL) { leftString = this->left->treeToString(); }
    else { leftString = "-"; }
    if(this->right != NULL) { rightString = this->right->treeToString(); }
    else { rightString = "-"; }

    return "("+leftString+" "+this->data+" "+rightString+")";
}