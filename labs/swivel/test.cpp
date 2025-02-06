#include "Set.h"
#include <iostream>

// Use this file to test your Set class.
// This file won't be graded - do whatever you want.

int main() {
  Set set;
  set.insert("r");
  set.insert("a");
  set.insert("t");
  set.swivel("r");
  set.print();
  set.swivel("a");
  set.print();
  set.swivel("r");
  set.print();
  set.swivel("g");
  set.print();
  std::cout << set.count();
  return 0;
}
