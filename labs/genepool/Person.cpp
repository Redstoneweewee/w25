#include "Person.h"
#include <iostream>

// Person Member Functions
Person::Person() {
    mMother = NULL;
    mFather = NULL;
}

Person::Person(std::string& name, Gender gender, Person* mother, Person* father) {
    mName   = name;
    mGender = gender;
    mMother = mother;
    mFather = father;
}


//Getter functions
const std::string& Person::name() const { return mName; }
Gender Person::gender() const { return mGender; }
Person* Person::mother() { return mMother; }
Person* Person::father() { return mFather; }
std::set<Person*> Person::children() { return mChildren; }


//Add child function
void Person::addChild(Person* child) {
    mChildren.insert(child);
}

//Relationship functions
std::set<Person*> Person::getThisAndAllAncestors() {
    std::set<Person*> people;
    people.insert(this);
    if(mMother != NULL) { people.merge(this->mMother->getThisAndAllAncestors()); }
    if(mFather != NULL) { people.merge(this->mFather->getThisAndAllAncestors()); }
    return people;
}
std::set<Person*> Person::getThisAndAllChildren() {
    std::set<Person*> people;
    people.insert(this);
    for(Person* child : mChildren) {
        people.merge(child->getThisAndAllChildren());
    }
    return people;
}

/**
 * Negative level = children
 *    ex. level = -1 --> children
 *    ex. level = -2 --> grandChildren
 * Positive level = parents
 *    ex. level =  1 --> parents
 *    ex. level =  2 --> grandParents
 * level = 0 --> gets itself
 */
std::set<Person*> Person::getPeopleAtLevel(int level, Gender gender, Person* initialPerson, SMod smod) {
    std::set<Person*> people;
    if(level == 0) {
        if((gender == Gender::ANY || gender == this->gender()) && isValidSMod(initialPerson, this, smod)) {
            people.insert(this);
        }
    }
    else if(level < 0) {
        for(Person* child : mChildren) {
            people.merge(child->getPeopleAtLevel(level+1, gender, initialPerson, smod));
        }
    }
    else {
        if(mMother != NULL) { people.merge(mMother->getPeopleAtLevel(level-1, gender, initialPerson, smod)); }
        if(mFather != NULL) { people.merge(mFather->getPeopleAtLevel(level-1, gender, initialPerson, smod)); }
    }
    return people;
}

std::set<Person*> Person::ancestors(PMod pmod) {
    std::set<Person*> people;
    if(pmod == PMod::MATERNAL && mMother != NULL) {
        people = this->mMother->getThisAndAllAncestors();
    }
    else if(pmod == PMod::PATERNAL && mFather != NULL) {
        people = this->mFather->getThisAndAllAncestors();
    }
    else if(mMother != NULL && mFather != NULL) {
        std::set<Person*> motherSide = this->mMother->getThisAndAllAncestors();
        std::set<Person*> fatherSide = this->mFather->getThisAndAllAncestors();
        motherSide.merge(fatherSide);
        people = motherSide;
    }
    return people;
}
std::set<Person*> Person::descendants() {
    return this->getThisAndAllChildren();
}
std::set<Person*> Person::sons() {
    return this->getPeopleAtLevel(-1, Gender::MALE);
}
std::set<Person*> Person::daughters() {
    return this->getPeopleAtLevel(-1, Gender::FEMALE);
}
std::set<Person*> Person::grandchildren() {
    return this->getPeopleAtLevel(-2, Gender::ANY);
}
std::set<Person*> Person::grandsons() {
    return this->getPeopleAtLevel(-2, Gender::MALE);
}
std::set<Person*> Person::granddaughters() {
    return this->getPeopleAtLevel(-2, Gender::FEMALE);
}


std::set<Person*> Person::parents(PMod pmod) {
    return this->getPeopleAtLevel(1, Gender::ANY);
}
std::set<Person*> Person::grandparents(PMod pmod) {
    return this->getPeopleAtLevel(2, Gender::ANY);
}
std::set<Person*> Person::grandfathers(PMod pmod) {
    return this->getPeopleAtLevel(2, Gender::MALE);
}
std::set<Person*> Person::grandmothers(PMod pmod) {
    return this->getPeopleAtLevel(2, Gender::FEMALE);
}



std::set<Person*> Person::siblings(PMod pmod, SMod smod) {
    std::set<Person*> people;
    people.merge(this->brothers(pmod, smod));
    people.merge(this->sisters(pmod, smod));
    return people;
}
std::set<Person*> Person::brothers(PMod pmod, SMod smod) {
    std::set<Person*> brothers;
    std::set<Person*> parents = this->getPeopleAtLevel(1, pModToGender(pmod), this, SMod::ANY);
    for(Person* parent : parents) {
        brothers.merge(parent->getPeopleAtLevel(-1, Gender::MALE, this, smod));
    }
    brothers.erase(this);
    return brothers;
    
}
std::set<Person*> Person::sisters(PMod pmod, SMod smod) {
    std::set<Person*> sisters;
    std::set<Person*> parents = this->getPeopleAtLevel(1, pModToGender(pmod), this, SMod::ANY);
    for(Person* parent : parents) {
        sisters.merge(parent->getPeopleAtLevel(-1, Gender::FEMALE, this, smod));
    }
    sisters.erase(this);
    return sisters;
    
}

std::set<Person*> Person::nephews(PMod pmod, SMod smod) {
    std::set<Person*> people;
    std::set<Person*> siblings = this->siblings(pmod, smod);
    for(Person* sibling : siblings) {
        people.merge(sibling->getPeopleAtLevel(-1, Gender::MALE));
    }
    return people;
    
}
std::set<Person*> Person::nieces(PMod pmod, SMod smod) {
    std::set<Person*> people;
    std::set<Person*> siblings = this->siblings(pmod, smod);
    for(Person* sibling : siblings) {
        people.merge(sibling->getPeopleAtLevel(-1, Gender::FEMALE));
    }
    return people;
    
}
std::set<Person*> Person::cousins(PMod pmod, SMod smod) {
    std::set<Person*> cousins;
    std::set<Person*> parents = this->getPeopleAtLevel(1, pModToGender(pmod));
    std::set<Person*> parentsSiblings;
    for(Person* parent : parents) {
        parentsSiblings.merge(parent->siblings(PMod::ANY, smod));
    }
    for(Person* parentsSibling : parentsSiblings) {
        cousins.merge(parentsSibling->children());
    }
    return cousins;
}


std::set<Person*> Person::aunts(PMod pmod, SMod smod) {
    std::set<Person*> people;
    std::set<Person*> parents = this->getPeopleAtLevel(1, pModToGender(pmod));
    for(Person* parent : parents) {
        people.merge(parent->sisters(PMod::ANY, smod));
    }
    return people;
    
}
std::set<Person*> Person::uncles(PMod pmod, SMod smod) {
    std::set<Person*> people;
    std::set<Person*> parents = this->getPeopleAtLevel(1, pModToGender(pmod));
    for(Person* parent : parents) {
        people.merge(parent->brothers(PMod::ANY, smod));
    }
    return people;
    
}


//Helper functions
Gender Person::pModToGender(PMod pmod) {
    Gender gender = Gender::ANY;
    if(pmod == PMod::MATERNAL)      { gender = Gender::FEMALE; }
    else if(pmod == PMod::PATERNAL) { gender = Gender::MALE; }
    return gender;
}

bool Person::isValidSMod(Person* person1, Person* person2, SMod smod) {
    if(smod == SMod::ANY) { return true; }
    if(person1 == NULL || person2 == NULL) { return false; }
    else if(smod == SMod::FULL) {
        if(person1->father() == NULL || person2->father() == NULL || person1->mother() == NULL || person2->mother() == NULL) { return false; } 
        if(person1->father() == person2->father() && person1->mother() == person2->mother()) { return true; }
        
    }
    else if(smod == SMod::HALF) {
        if(person1->father() == person2->father() && person1->mother() == person2->mother()) { return false; }
        else if(person1->father() == person2->father() && person1->father() != NULL) { return true; }
        else if(person1->mother() == person2->mother() && person1->mother() != NULL) { return true; }   
    }
    return false;
}