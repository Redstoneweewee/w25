#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "Point.h"
#include "Tile.h"
#include "DisjointSet.h"
using namespace std;


class Map {
    // Member Variables
    size_t height;
    size_t length;
    vector<vector<char>> charMap;
    vector<vector<Tile*>> tileMap;
    /** This one has one set = all tiles in one region*/
    DisjointSet regionalDisjointSet;
    
    /** This one has sets of connected regions*/
    //DisjointSet mapDisjointSet;
public:
    Map(std::istream& stream);
    ~Map();

    std::string route(Point src, Point dst);
    void printMap() const;
    void printPerimeter() const;

private:
    void initializeCharMap(std::istream& stream);
    void initializeTileMap();
    void initializeRegionalDisjointSet();

    void initializeAndSetNeighbors(Tile* tile);
    bool isPointValid(const Point& p) const;
    bool isPointReachable(Point& p, bool isStartingPoint) const;
    std::array<Point, 4> calculateNeighborPoints(const Point& p);
};

#endif
