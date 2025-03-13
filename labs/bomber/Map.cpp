#include "Map.h"
#include <sstream>
#include <array>
#include "Errors.h"

#pragma region Members
// Member Functions ------------------------------------------------------------------------------
Map::Map(std::istream& stream) {
    height = 0;

    initializeCharMap(stream);
    initializeTileMap();
    initializeRegionalDisjointSet();
}

Map::~Map() {
    for(vector<Tile*> row : tileMap) {
        for(Tile* p : row) {
            delete p;
        }
    }
}


std::string Map::route(Point src, Point dst) {
    if (!isPointReachable(src, true) || !isPointReachable(dst, false)) {
        throw PointError(src);
    }
    return "";
}

void Map::printRegions() {
    std::unordered_map<Tile*, char> allParents;
    char startingChar = 'A';
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            Tile* parent = regionalDisjointSet.find(tile);
            if(parent != NULL && allParents.find(parent) == allParents.end()) {
                allParents.insert({parent, startingChar});
                startingChar++;
            }
        }
    }
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            auto it = allParents.find(regionalDisjointSet.find(tile));
            if(it != allParents.end()) {
                std::cout << it->second;
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
            std::cout << tile->isPerimeter;
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
            if(tileMap[neighborPoints[i].y][neighborPoints[i].x] == NULL) {
                Tile* neighbor = new Tile(charMap[neighborPoints[i].y][neighborPoints[i].x], neighborPoints[i]);
                initializeAndSetNeighbors(neighbor);
                tile->neighbors[i] = neighbor;
            }
            else {
                tile->neighbors[i] = tileMap[neighborPoints[i].y][neighborPoints[i].x];
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
            std::cout << "united " << tile->point << " with " << tile->neighbors[i]->point << "\n";
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

