#include "Map.h"
#include <sstream>
#include <array>
#include "Errors.h"
#include "Region.h"

#pragma region Members
// Member Functions ------------------------------------------------------------------------------
Map::Map(std::istream& stream) {
    height = 0;

    initializeCharMap(stream);
    initializeTileMap();
    initializeRegionalDisjointSet();
    initializeRegions();
    initializeMapDisjointSet();
}

Map::~Map() {
    for(vector<Tile*> row : tileMap) {
        for(Tile* p : row) {
            delete p;
        }
    }
    for(auto region : regions) {
        delete region.second;
    }
}


std::string Map::route(Point src, Point dst) {
    if (!isPointReachable(src, true) || !isPointReachable(dst, false)) {
        throw PointError(src);
    }
    return "";
}

void Map::printMapDisjointSet() {
    for(auto it : regions) {
        Region* region = it.second;
        std::cout << "region " << region->regionName << "'s parent: " << mapDisjointSet.find(region)->regionName << "\n";
    }
    std::cout << "\n";
}
void Map::printRegionConnections() {
    for(auto it : regions) {
        Region* region = it.second;
        std::cout << "region " << region->regionName << ": ";
        for(Region::Connection* connection : region->connections) {
            std::cout << " <-" << connection->weight << "-> " << connection->getOther(region)->regionName << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
void Map::printRegionalDisjointSet() {
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            if(regionalDisjointSet.find(tile) != NULL) {
                std::cout << regionalDisjointSet.find(tile)->point;
            }
            else {
                std::cout << "      ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
void Map::printRegionsFromRegions() {
    char startingChar = 'A';
    for(auto region : regions) {
        std::cout << "region " << startingChar << ":\n";
        std::cout << "perimeter: ";
        for(Tile* perim : region.second->perimeter) {
            std::cout << perim->point << ", ";
        }
        startingChar++;
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Map::printRegionsFromDisjointSet() {
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            Tile* parentTile = regionalDisjointSet.find(tile);
            if(parentTile != NULL) {
                //std::cout << "find: " << parentTile->point << "\n";
                auto it = regions.find(parentTile);
                std::cout << it->second->regionName;
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
void Map::printPerimeter() const {
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            std::cout << (tile->isPerimeter ? "X" : " ");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
void Map::printMap() const {
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            std::cout << tile->type;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
/*
void Map::printMap() const {
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            std::string str = "[";
            for(size_t i=0; i<4; i++) {
                if(tile->neighbors[i] != NULL) {
                    str += to_string(i);
                    str += tile->neighbors[i]->type;
                }
            }
            str += "]";
            std::cout << tile->type << " " << str << " ";
        }
        std::cout << "\n";
    }
}
*/
#pragma endregion





#pragma region Init
//Partial initialization functions ------------------------------------------------------------------------------
void Map::initializeCharMap(std::istream& stream) {
    string currentLine;

    while (getline(stream, currentLine, '\n')) {
        length = currentLine.size();
        charMap.push_back(vector<char>{});
        for (char c : currentLine) {
            charMap[height].push_back(c);
        }
        height++;
    }
}
#pragma endregion

//must be done after charMap init
void Map::initializeTileMap() {
    for(size_t i=0; i<height; i++) {
        tileMap.push_back(vector<Tile*>{});
        for(size_t j=0; j<length; j++) {
            tileMap[i].push_back(NULL);
        }
    }
    tileMap[0][0] = new Tile(charMap[0][0], Point(0, 0));
    initializeAndSetNeighbors(tileMap[0][0]);
}

/**
 * must be done after the tile has been initialized
 */
void Map::initializeAndSetNeighbors(Tile* tile) {
    tileMap[tile->point.y][tile->point.x] = tile;

    std::array<Point, 4> neighborPoints = calculateNeighborPoints(tile->point);
    for(size_t i=0; i<4; i++) {
        if(isPointValid(neighborPoints[i])) {
            //std::cout << "testing point (" << neighborPoints[i].x << ", " << neighborPoints[i].y << ")\n";
            if(getTile(neighborPoints[i]) == NULL) {
                Tile* neighbor = new Tile(charMap[neighborPoints[i].y][neighborPoints[i].x], neighborPoints[i]);
                initializeAndSetNeighbors(neighbor);
                tile->neighbors[i] = neighbor;
            }
            else {
                tile->neighbors[i] = getTile(neighborPoints[i]);
            }

            if((tile->type == '.' || tile->type == '*') && tile->neighbors[i]->type == '#') {
                tile->setIsPerimeter();
            }
        }
    }
}   

//must be done after tileMap init
void Map::initializeRegionalDisjointSet() {
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            if(tile->type == '.' || tile->type == '*') {
                createRegionFromTile(tile);
            }
        }
    }
}

void Map::createRegionFromTile(Tile* tile) {
    regionalDisjointSet.add(tile);
    for(size_t i=0; i<4; i++) {
        if(tile->neighbors[i] != NULL && regionalDisjointSet.find(tile->neighbors[i]) != NULL && 
          (tile->neighbors[i]->type == '.' || tile->neighbors[i]->type == '*')) {
            regionalDisjointSet.unite(tile, tile->neighbors[i]);
            //cannot just return here because it can lead to disconnected adjacent sets
            //std::cout << "united " << tile->point << " with " << tile->neighbors[i]->point << "\n";
        }
    }
}


//must be done after regionalDisjointSet init
void Map::initializeRegions() {
    char startingChar = 'A';               //Mainly for testing purporses, not actually necessary
    for(Tile* parentTile : regionalDisjointSet.allParents) {
        Region* region = new Region;
        region->regionName = startingChar; //Mainly for testing purporses, not actually necessary
        for(vector<Tile*> row : tileMap) {
            for(Tile* tile : row) {
                if(regionalDisjointSet.find(tile) == parentTile) {
                    if(tile->isPerimeter) {
                        region->perimeter.push_back(tile);
                    }
                    if(tile->type == '*') {
                        region->bombs.push_back(tile);
                    }
                }
            }
        }
        regions.insert({parentTile, region});
        startingChar++;                    //Mainly for testing purporses, not actually necessary
    }
}


//must be done after regions init
void Map::initializeMapDisjointSet() {
    for(auto it : regions) {
        mapDisjointSet.add(it.second);
        createRegionConnections(it.second);
    }
}

void Map::createRegionConnections(Region* region) {
    for(Tile* testingTile : region->perimeter) {
        /**
         * First probe:  left, -x,  0
         * Second probe: up,    0, -y
         * Third probe:  right, x,  0
         * Fourth probe: down,  0,  y
         */
        for(size_t i=0; i<4; i++) {
            Point nextPoint = testingTile->point;
            size_t numOfWalls = 0;
            int xInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
            int yInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
            while(true) {
                nextPoint.x += xInc;
                nextPoint.y += yInc;
                if(!isPointValid(nextPoint)) { break; }
                //std::cout << "checking point " << nextPoint << "\n";
                Tile* nextTile = getTile(nextPoint);
                if(nextTile->type == '~') { /*std::cout << "1\n";*/ break; }
                else if(nextTile->type == '#') { numOfWalls++; } //this has to go before the next statement because walls are not part of the disjoint sets
                else if(regionalDisjointSet.connected(testingTile, nextTile)) { /*std::cout << "2\n";*/ break; }
                //If none of those are true, then we have reached a new region
                else {
                    Region* otherRegion = getRegion(nextTile);
                    //For optimal path
                    if(region->connectedRegions.find(otherRegion) != region->connectedRegions.end()) {
                        for(Region::Connection* connection : region->connections) {
                            if(connection->getOther(region) == otherRegion && connection->weight > numOfWalls) {
                                connection->weight = numOfWalls;
                                break;
                            }
                        }
                    }
                    //For new path
                    if(region->connectedRegions.find(otherRegion) == region->connectedRegions.end()) {
                        //std::cout << "3\n";
                        Region::Connection* connection = new Region::Connection{numOfWalls, region, testingTile, otherRegion, nextTile};
                        region->connections.insert(connection);
                        otherRegion->connections.insert(connection);
                        region->connectedRegions.insert(otherRegion);
                        otherRegion->connectedRegions.insert(region);
                        mapDisjointSet.add(otherRegion);
                        mapDisjointSet.unite(region, otherRegion);
                    }
                    else {
                        //std::cout << "4\n";
                    }
                    //If not true, the regions are already connected, so no need to reconnect them
                    break;
                }
            }
        }
    }
}






#pragma region Helpers
//Helper functions ------------------------------------------------------------------------------

bool Map::isPointValid(const Point& p) const {
    if(p.x < 0 || p.y < 0) { return false; }
    if(p.x >= (int)length || p.y >= (int)height) { return false; }
    return true;
}

bool Map::isPointReachable(Point& p, bool isStartingPoint) const {
    if(!isPointValid(p)) { return false; }
    if(charMap[p.x][p.y] == '~') { return false; }
    if(isStartingPoint && charMap[p.x][p.y] == '#') { return false; }
    return true;
}

Tile* Map::getTile(const Point& p) const {
    return tileMap[p.y][p.x];
}

/**
 * Returns the region of any tile. If is wall or water, returns NULL.
 */
Region* Map::getRegion(Tile* t) {
    Tile* parentTile = regionalDisjointSet.find(t);
    if(parentTile == NULL) { return NULL; }
    return regions.find(parentTile)->second;
}

/**
 * Returns an array of size 4
 * [0] = left neighbor pt
 * [1] = top neighbor pt
 * [2] = right neighbor pt
 * [3] = bottom neighbor pt
 */
std::array<Point, 4> Map::calculateNeighborPoints(const Point& p) {
    return {
        Point(p.x-1, p.y),
        Point(p.x, p.y-1),
        Point(p.x+1, p.y),
        Point(p.x, p.y+1)
    };
}



#pragma endregion

