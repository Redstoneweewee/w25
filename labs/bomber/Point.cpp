#include "Point.h"

Point& Point::operator=(const Point& other) {
  x = other.x;
  y = other.y;
  return *this;
}

std::ostream& operator << (std::ostream& stream, const Point& point) {
  return stream << '(' << point.x << ", " << point.y << ')';
}

std::istream& operator >> (std::istream& stream, Point& point) {
  return stream >> point.x >> point.y;
}