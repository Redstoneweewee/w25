#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>
#include "Point.h"
using namespace std;


class Map {
    // Member Variables
    size_t height;
    size_t length;
    vector<vector<char>> data_map;
public:
    Map(std::istream& stream);
    // ~Map();

    //std::string route(Point src, Point dst);
    void printMap() const;
};

#endif
