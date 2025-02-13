#include "GenePool.h"

// GenePool Member Functions
GenePool::GenePool(std::istream& stream) {

}

GenePool::~GenePool() {

}

std::set<Person*> GenePool::everyone() const {
    return mEveryone;
}

Person* GenePool::find(const std::string& name) const {
    Person* temp = new Person();
    return *mEveryone.find(temp);
}