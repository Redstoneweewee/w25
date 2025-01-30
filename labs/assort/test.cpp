#include <iostream>
#include "List.h"

// Use this file to test your List class!
// This file won't be graded - do whatever you want.

int main() {
    List list;
    list.insert("china");
    list.insert("russia");
    list.insert("BOLIVIA");
    list.insert("zathary");
    list.insert("ZAthary");
    list.insert("russia");
    list.print();

    list.remove(2);
    list.remove(2);
    list.remove(2);
    list.remove(2);
    list.print();
    return 0;
}