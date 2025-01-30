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
    list.print(true);

    List list2(list);
    list2.print();
    list2.print(true);
    return 0;
}