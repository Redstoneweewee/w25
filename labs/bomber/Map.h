#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "Point.h"
#include "Tile.h"
using namespace std;


class Map {
    // Member Variables
    size_t height;
    size_t length;
    vector<vector<char>> charMap;
    vector<vector<Tile*>> tileMap;
public:
    Map(std::istream& stream);
    ~Map();

    std::string route(Point src, Point dst);
    void printMap() const;

private:
    void initializeCharMap(std::istream& stream);
    void initializeTileMap();
    void initializeAndSetNeighbors(Tile* tile);

    bool isPointValid(const Point& p) const;
    bool isPointReachable(Point& p, bool isStartingPoint) const;
    std::array<Point, 4> calculateNeighborPoints(const Point& p);
};

#endif
