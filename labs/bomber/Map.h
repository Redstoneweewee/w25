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

    /** This is used to initialize all possible 2-region connections */
    unordered_map<Region*, unordered_set<Region*>> hasDirectConnection;
public:
    Map(std::istream& stream);
    ~Map();

    //std::string route(Point src, Point dst);
    void printMap() const;
    void printPerimeter() const;
    void printRegionsFromDisjointSet();
    void printRegionsFromRegions();
    void printRegionalDisjointSet();
    void printRegionConnections();
    void printMapDisjointSet();
    void printScores() const;

private:
    void initializeCharMap(std::istream& stream);
    void initializeTileMap();
    void initializeRegionalDisjointSet();
    void initializeRegions();
    void initializeMapDisjointSet();
    void initializeTripletConnections();

    void initializeAndSetNeighbors(Tile* tile);
    void createRegionFromTile(Tile* tile);
    void createRegionConnections(Region* region);

    bool isPointValid(const Point& p) const;
    bool isPointReachable(Point& p, bool isStartingPoint) const;
    Tile* getTile(const Point& p) const;
    Region* getRegion(Tile* t);
    std::array<Point, 4> calculateNeighborPoints(const Point& p);
    vector<vector<size_t>> countTileSteps(size_t m, size_t n);
    void calculateOptimalTriplet(vector<Region*> triplet);
    
    Point getTopLeft(Tile* tile1, Tile* tile2);
    Point getBottomRight(Tile* tile1, Tile* tile2);
    vector<Tile*> createTileBox(Point topLeft, Point bottomRight);
    //vector<Region*> regionPathFinidng(Point& src, Point& dst, int bomb_count, unordered_map<Region*, Region*>& visited_nodes);
};

#endif
