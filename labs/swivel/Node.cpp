#include "Node.h"

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