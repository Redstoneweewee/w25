//#include "List.h"
#include <iostream>

bool isStringAsciiLesserOrEqual(const std::string& str1, const std::string& str2);
// Use this file to test your List class!
// This file won't be graded - do whatever you want.

int main() {
  //List list;

  std::cout << isStringAsciiLesserOrEqual("", "") << "\n";
  std::cout << isStringAsciiLesserOrEqual("WORMF", "WORMF") << "\n";
  std::cout << isStringAsciiLesserOrEqual("wormf", "wormd") << "\n";
  std::cout << isStringAsciiLesserOrEqual("a", "A") << "\n";
  std::cout << isStringAsciiLesserOrEqual("A", "a") << "\n";

  return 0;
}

bool isStringAsciiLesserOrEqual(const std::string& str1, const std::string& str2) {
    if(str1.size() == 0) { return true; }
    if(str2.size() == 0) { return false; }

    size_t stringIndex = 0;
    //compares the two strings. If the characters are equal, then move onto the next character to compare.
    //If we have reached the end of either string, then stop the loop
    while(str1.size() > stringIndex && str2.size() > stringIndex && str1[stringIndex] == str2[stringIndex]) {
        stringIndex++;
    }
    //If we have reached the end of either string and their values are the same,
    //returns true if str2 is longer or equal, false otherwise
    if(str1[stringIndex] == str2[stringIndex]) {
        if(str1.size() <= str2.size()) { return true; }
        return false;
    }
    else if(str1[stringIndex] < str2[stringIndex]) {
        return true;
    }
    return false;
}