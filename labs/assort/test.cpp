#include <iostream>
#include "List.h"

// Use this file to test your List class!
// This file won't be graded - do whatever you want.

int main() {
    List list;
    list.insert("china");
    list.print();
    list.insert("russia");
    list.print();
    list.insert("BOLIVIA");
    list.print();
    list.insert("zathary");
    list.print();
    list.insert("ZAthary");
    list.print();
    list.insert("russia");
    list.print();
    return 0;
}