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


void Map::printMap() const {
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            std::cout << tile->type;
        }
        std::cout << "\n";
    }
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
 * returns the right and bottom neighbors [1, 2]
 * 
 * we only care about right and bottom neighbors cuz 
 * we are initializing them from top left to bottom right
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
 * [0] = top neighbor pt
 * [1] = right neighbor pt
 * [2] = bottom neighbor pt
 * [3] = left neighbor pt
 */
std::array<Point, 4> Map::calculateNeighborPoints(const Point& p) {
    return {
        Point(p.x, p.y-1),
        Point(p.x+1, p.y),
        Point(p.x, p.y+1),
        Point(p.x-1, p.y)
    };
}



#pragma endregion

