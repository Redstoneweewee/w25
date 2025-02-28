#include "List.h"
#include <iostream>
#include <string>

int main() {

    List testList;
    testList.insertAtTail(new Node("a", 1));
    testList.insertAtTail(new Node("b", 2));
    testList.insertAtTail(new Node("c", 3));
    testList.insertAtTail(new Node("d", 4));
    testList.insertAtTail(new Node("e", 5));
    testList.insertAtTail(new Node("f", 6));
    testList.insertAtTail(new Node("g", 7));
    testList.insertAtTail(new Node("h", 8));
    testList.printAll();
    testList.remove("d");
    std::cout << std::endl;
    testList.printAll();
    testList.remove("b");
    testList.insertAtTail(new Node("b", 2));
    std::cout << std::endl;
    testList.printAll();
    return 0;
}