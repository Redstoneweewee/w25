#include "GenePool.h"

  
#include <iostream>
// GenePool Member Functions
GenePool::GenePool(std::istream& stream) {
    while(!stream.eof()) {
        std::string line;
        std::getline(stream, line);
        if(line.substr(0, 1) == "#") { continue; }
        if(line == "") { continue; }
        //std::cout << "line: " << line << std::endl;
        std::string attributes[4];
        for(int i=0; i<4; i++) {
            attributes[i] = line.substr(0, line.find('\t'));
            line.erase(0, line.find('\t')+1);
        }
        //std::cout << "name: " << attributes[0] << ", gender: " << attributes[1] << ", mother: " << attributes[2] << ", father: " << attributes[3] << std::endl;

        /**
         * attributes[0] = name
         * attributes[1] = gender
         * attributes[2] = mother name
         * attributes[3] = father name
         */

        Gender gender = Gender::MALE;
        Person* mother = NULL;
        Person* father = NULL;
        if(attributes[1] == "female") { gender = Gender::FEMALE; }
        if(attributes[2] != "???") { mother = mEveryone[attributes[2]]; }
        if(attributes[3] != "???") { father = mEveryone[attributes[3]]; }

        Person* person = new Person(attributes[0], gender, mother, father);
        
        if(attributes[2] != "???") { mEveryone[attributes[2]]->addChild(person); }
        if(attributes[3] != "???") { mEveryone[attributes[3]]->addChild(person); }

        mEveryone[attributes[0]] = person;
        //std::cout << "added person: name: " << attributes[0] << ", gender: " << attributes[1] << ", mother: " << attributes[2] << ", father: " << attributes[3] << std::endl;
    }
}

GenePool::~GenePool() {
    for(std::map<std::string, Person*>::const_iterator iterator = mEveryone.begin(); iterator != mEveryone.end(); iterator++) {
        delete iterator->second;
    }
}

std::set<Person*> GenePool::everyone() const {
    std::set<Person*> everyone;
    for(std::map<std::string, Person*>::const_iterator iterator = mEveryone.begin(); iterator != mEveryone.end(); iterator++) {
        everyone.insert(iterator->second);
    }
    return everyone;
}

Person* GenePool::find(const std::string& name) const {
    std::map<std::string, Person*>::const_iterator iterator = mEveryone.find(name);
    return iterator->second;
}