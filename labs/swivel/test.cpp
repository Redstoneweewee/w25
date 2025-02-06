#include "Set.h"
#include <iostream>

// Use this file to test your Set class.
// This file won't be graded - do whatever you want.

int main() {
  Set set;
  set.insert("y");
  set.insert("z");
  set.insert("n");
  set.insert("e");
  set.insert("r");
  set.insert("p");
  set.insert("t");
  set.print();
  set.remove("n");
  set.print();
  
  return 0;
}
