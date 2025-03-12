#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point {
  int x;  // Latitude
  int y;  // Longitude

  Point() {
    // Beware of junk values!
  }

  Point(int x, int y) {
    this->x = x;
    this->y = y;
  }

  Point(const Point& other) {
    this->x = other.x;
    this->y = other.y;
  }
};

// Operators for easier (and consistent) input and output:
std::ostream& operator << (std::ostream& stream, const Point& point);
std::istream& operator >> (std::istream& stream, Point& point);

#endif
