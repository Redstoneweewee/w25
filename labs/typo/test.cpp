#include "Heap.h"
#include <iostream>

// Use this file to test your Heap class!
// This file won't be graded - do whatever you want.

int main() {
    Heap heap(8);
    heap.push("Monday", 10);
    heap.push("Tuesday", 20);
    heap.push("Wednesday", 30);
    heap.push("Thursday", 40);
    heap.push("Friday", 50);
    heap.push("Saturday", 37);
    heap.push("Sunday", 35);
    for(size_t i=0; i<heap.count(); i++) {
        std::cout << "index " << i << ": value: " << heap.lookup(i).value << ", score: " << heap.lookup(i).score << "\n";
    }
    heap.pop();
    std::cout << "popped\n\n";
    for(size_t i=0; i<heap.count(); i++) {
        std::cout << "index " << i << ": value: " << heap.lookup(i).value << ", score: " << heap.lookup(i).score << "\n";
    }
    std::cout << "This program doesn't do anything interesting yet...\n";

    return 0;
}
