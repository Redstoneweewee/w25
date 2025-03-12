#include "Point.h"

std::ostream& operator << (std::ostream& stream, const Point& point) {
  return stream << '(' << point.x << ", " << point.y << ')';
}

std::istream& operator >> (std::istream& stream, Point& point) {
  return stream >> point.x >> point.y;
}
