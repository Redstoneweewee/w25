#include "List.h"
#include "Index.h"
#include <iostream>
#include <string>

int main() {

    List testList;
    testList.insertAtTail(new Node("a", 106));
    testList.insertAtTail(new Node("b", 2));
    testList.insertAtTail(new Node("c", 3));
    testList.insertAtTail(new Node("d", 4));
    testList.insertAtTail(new Node("e", 5));
    testList.insertAtTail(new Node("f", 6));
    testList.insertAtTail(new Node("g", 7));
    testList.insertAtTail(new Node("h", 8));
    testList.printAll();

    Index testIndex;
    testIndex.insert(testList.find("a"));
    testIndex.insert(testList.find("b"));
    testIndex.insert(testList.find("c"));
    testIndex.insert(testList.find("d"));
    testIndex.insert(testList.find("e"));
    testIndex.insert(testList.find("f"));
    testIndex.insert(testList.find("g"));
    testIndex.insert(testList.find("h"));
    std::string a = "a";
    std::string b = "b";
    std::string c = "c";
    std::string d = "d";
    std::string e = "e";
    std::string f = "f";
    std::string g = "g";
    std::string h = "h";
    testIndex.remove(a);
    testIndex.insert(testList.find("a"));
    testIndex.remove(a);
    testIndex.insert(testList.find("a"));
    testIndex.remove(a);
    std::cout << "value for a: " << testIndex.lookup(a) << "\n";
    std::cout << "value for b: " << testIndex.lookup(b) << "\n";
    std::cout << "value for c: " << testIndex.lookup(c) << "\n";
    std::cout << "value for d: " << testIndex.lookup(d) << "\n";
    std::cout << "value for e: " << testIndex.lookup(e) << "\n";
    std::cout << "value for f: " << testIndex.lookup(f) << "\n";
    std::cout << "value for g: " << testIndex.lookup(g) << "\n";
    std::cout << "value for h: " << testIndex.lookup(h) << "\n";
    return 0;
}