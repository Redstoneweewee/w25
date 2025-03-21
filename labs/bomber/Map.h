#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <set>
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


    std::string route(Point src, Point dst);

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

    void oldInitializeMapDisjointSet();
    void oldCreateRegionConnections(Region* region);

    bool useNew();
    void initializeAndSetNeighbors(Tile*& tile);
    void createRegionFromTile(Tile* tile);
    void createRegionConnections(Region* region);
    bool tryReachThirdRegion(size_t& returnDistance, Tile* startingTile, int xInc, int yInc, Region* region1, Region* region2, Region* region3);
    void connectRegions(bool canDuplicate, size_t distance, Region* region1, Region* region2, Tile* parentTile1, Tile* parentTile2);
    bool isPointValid(const Point& p) const;
    bool isPointReachable(Point& p, bool isStartingPoint) const;
    Tile* getTile(const Point& p) const;
    Region* getRegion(Tile* t);
    std::array<Point, 4> calculateNeighborPoints(const Point& p);
    vector<vector<size_t>> countTileSteps(size_t m, size_t n);
    bool isUsefulPath(Region* region, Region* otherRegion, Tile* tile1, Tile* tile2);
    set<pair<Tile*, array<Region*, 3>>> calculateOptimalTriplet(vector<Region*> triplet);
    
    Point getTopLeft(Tile* tile1, Tile* tile2);
    Point getBottomRight(Tile* tile1, Tile* tile2);
    vector<Tile*> createTileBox(Point topLeft, Point bottomRight);
    
    vector<Region::Connection*> regionPathFinding(Point& src, Point& dst, size_t bomb_count, unordered_map<Region*, Region*>& visited_connections);
    vector<Tile*> pointPathFinding(Tile* start, Tile* end, size_t& bomb_count, unordered_map<Tile*, Tile*>& visited_tiles);
};

#endif
