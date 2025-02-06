#include "Set.h"
#include <iostream>

// Use this file to test your Set class.
// This file won't be graded - do whatever you want.

int main() {
  Set set;
  set.insert("r");
  set.insert("a");
  set.insert("t");
  set.insert("d");
  set.insert("v");
  set.insert("e");
  set.print();
  set.swivel("a");
  set.print();

  Set set2(set);
  set2.print();
  set.clear();
  
  set.print();
  set2.print();
  return 0;
}
