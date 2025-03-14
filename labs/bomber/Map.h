#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include "Point.h"
#include "Tile.h"
#include "DisjointSet.h"
#include "Region.h"
using namespace std;


class Map {
    // Member Variables
    size_t height;
    size_t length;
    vector<vector<char>> charMap;
    vector<vector<Tile*>> tileMap;
    unordered_map<Tile*, Region*> regions;

    /** This one has one set = all tiles in one region*/
    DisjointSet<Tile*> regionalDisjointSet;
    
    /** This one has sets of connected regions*/
    DisjointSet<Region*> mapDisjointSet;
public:
    Map(std::istream& stream);
    ~Map();

    std::string route(Point src, Point dst);
    void printMap() const;
    void printPerimeter() const;
    void printRegionsFromDisjointSet();
    void printRegionsFromRegions();
    void printRegionalDisjointSet();
    void printRegionConnections();
    void printMapDisjointSet();

private:
    void initializeCharMap(std::istream& stream);
    void initializeTileMap();
    void initializeRegionalDisjointSet();
    void initializeRegions();
    void initializeMapDisjointSet();

    void initializeAndSetNeighbors(Tile* tile);
    void createRegionFromTile(Tile* tile);
    void createRegionConnections(Region* region);

    bool isPointValid(const Point& p) const;
    bool isPointReachable(Point& p, bool isStartingPoint) const;
    Tile* getTile(const Point& p) const;
    Region* getRegion(Tile* t);
    std::array<Point, 4> calculateNeighborPoints(const Point& p);
};

#endif
