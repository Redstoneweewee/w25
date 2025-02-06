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
  set.remove("r");
  set.remove("e");
  set.remove("y");
  set.remove("n");
  set.remove("t");
  set.remove("z");
  set.remove("p");
  set.print();
  
  return 0;
}
