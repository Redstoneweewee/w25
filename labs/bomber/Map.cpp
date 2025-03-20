#include "Map.h"
#include <sstream>
#include <array>
#include <cmath>
#include <map>
#include "Errors.h"
#include "Region.h"


// Member Functions ------------------------------------------------------------------------------
Map::Map(std::istream& stream) {
    height = 0;

    initializeCharMap(stream);
    initializeTileMap();
    initializeRegionalDisjointSet();
    initializeRegions();
    initializeMapDisjointSet();
    initializeTripletConnections();
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

 /* 
 Member Variables
 size_t height;
 size_t length;
 vector<vector<char>> charMap;
 vector<vector<Tile*>> tileMap;
 unordered_map<Tile*, Region*> regions;

 -This one has one set = all tiles in one region
 DisjointSet<Tile*> regionalDisjointSet;

 -This one has sets of connected regions
 DisjointSet<Region*> mapDisjointSet;
 */



// Routing Functions ------------------------------------------------------------------------------

/*
std::string Map::route(Point src, Point dst) {
    if (!isPointReachable(src, true) || !isPointReachable(dst, false)) {
        throw PointError(src);
    }
    unordered_map<Region*, Region*> visited_nodes;  //int is no of bombs needed to get to region
    int bomb_count = regions.at(tileMap[src.y][src.x])->bombs.size();
    vector<Region*> region_route_to_dst = regionPathFinidng(src, dst,bomb_count,visited_nodes);


    return "";
}

vector<Region*> Map::regionPathFinidng(Point& src, Point& dst, int bomb_count, unordered_map<Region*, Region*>& visited_nodes){
    Region* starting_region = regions.at(tileMap[src.y][src.x]); //do throw-catch checking system
    Region* ending_region = regions.at(tileMap[dst.y][dst.x]);

    //Tile* starting_tile = tileMap[src.y][src.x];
    //Tile* ending_tile = tileMap[dst.y][dst.x];

    vector<Region*> region_route;
                //make variables for result from getother()
        for (Region::Connection* current_connection: starting_region->connections){
            //lines of code to check if neighbor region is already in visited (no backtracking allowed)
            int current_bomb_count = bomb_count;
            auto iterator = visited_nodes.find(current_connection->getOther(starting_region));
            if (iterator != visited_nodes.end()){
                continue;
            }
            if (current_connection->getOther(starting_region) == ending_region ){
                if (bomb_count < current_connection->weight){
                continue; //indicates that this connection couldn't get to dst
                }
                region_route.push_back(current_connection->getOther(starting_region));
                region_route.push_back(starting_region);
            return region_route;
            } 
            else if (bomb_count >= current_connection->weight){
            current_bomb_count = current_bomb_count - current_connection->weight + current_connection->getOther(starting_region)->bombs.size();
            visited_nodes[starting_region] = starting_region;
            vector<Region*> updated_route = regionPathFinidng(current_connection->getOther(starting_region)->perimeter[0]->point, dst,current_bomb_count, visited_nodes);
                if (updated_route.size() == 0)
                {
                continue;
                }
                region_route = updated_route;
                region_route.push_back(starting_region);
                return region_route;
            }
            continue;
        }
    return region_route;
}
*/

// Printing Functions ------------------------------------------------------------------------------

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
            std::cout << connection->getSelfTile(region)->point << "<-" << connection->weight << "->" << connection->getOtherTile(region)->point << " " << connection->getOther(region)->regionName << ", ";
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
    bool first = true;
    size_t rowNum = 0;
    for(vector<Tile*> row : tileMap) {
        if(first) {
            cout << "  ";
            for(size_t i=0; i<row.size(); i++) {
                if(i % 10 == 0) { cout << i/10; }
                else { cout << " "; }
            }
            cout << "\n";
            cout << "  ";
            for(size_t i=0; i<row.size(); i++) {
                cout << i % 10;
            }
            cout << "\n";
            first = false;
        }
        if(rowNum % 10 == 0) { cout << rowNum/10; }
        else { cout << " "; }
        cout << rowNum % 10;
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
        rowNum++;
        std::cout << "\n";
    }
    std::cout << "\n";
}
void Map::printPerimeter() const {
    unordered_set<Tile*> perimeterTiles;
    for(auto it : regions) {
        Region* region = it.second;
        for(Tile* perimeter : region->perimeter) {
            perimeterTiles.insert(perimeter);
        }
    }
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            if(perimeterTiles.find(tile) != perimeterTiles.end()) {
                std::cout << "X";
            }
            else {
                std::cout << " ";
            }
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

void Map::printScores() const {
    for(vector<Tile*> row : tileMap) {
        for(Tile* tile : row) {
            std::cout << tile->score << "\t";
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
                tile->setIsNearPerimeterWall();
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
        region->parentTile = parentTile;
        region->regionName = startingChar; //Mainly for testing purporses, not actually necessary
        for(vector<Tile*> row : tileMap) {
            for(Tile* tile : row) {
                if(regionalDisjointSet.find(tile) == parentTile) {
                    if(tile->isNearPerimeterWall) {
                        for(size_t i=0; i<4; i++) {
                            if(tile->neighbors[i] != NULL && tile->neighbors[i]->type == '#') {
                                region->perimeter.insert(tile->neighbors[i]);
                            }
                        }
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
    }
    for(auto it : regions) {
        createRegionConnections(it.second);
    }
}

void Map::createRegionConnections(Region* region) {
    for(auto it : regions) {
        map<Tile*, vector<Tile*>> minPaths; //key = testingTiles, value = vector of otherTiles
        size_t minDistance = 9999999;
        Region* otherRegion = it.second;
        if(otherRegion == region || hasDirectConnection[region].count(otherRegion) > 0) { continue; }
        for(Tile* testingTile : region->perimeter) {

            for(Tile* otherTile : otherRegion->perimeter) {
                if(!isUsefulPath(region, otherRegion, testingTile, otherTile)) { continue; }
                size_t distance = abs(otherTile->point.x-testingTile->point.x) + abs(otherTile->point.y-testingTile->point.y);
                //cout << "distance between " << testingTile->point << " and " << otherTile->point << ": " << distance << "\n";
                if(distance < minDistance) {
                    minPaths.clear();
                    minPaths.insert({testingTile, vector<Tile*>{otherTile}});
                    minDistance = distance;
                }
                else if(distance == minDistance) {
                    if(minPaths.find(testingTile) != minPaths.end()) {
                        minPaths.find(testingTile)->second.push_back(otherTile);
                    }
                    else {
                        minPaths.insert({testingTile, vector<Tile*>{otherTile}});
                    }
                }
            }
        }
        for(auto it : minPaths) {
            mapDisjointSet.unite(region, otherRegion);
            hasDirectConnection[region].insert(otherRegion);
            hasDirectConnection[otherRegion].insert(region);
            Tile* testingTile = it.first;
            for(Tile* otherTile : it.second) {
                Region::Connection* connection = new Region::Connection{minDistance+1, region, testingTile, otherRegion, otherTile};
                region->connections.insert(connection);
                otherRegion->connections.insert(connection);
                region->connectedRegions.insert(otherRegion);
                otherRegion->connectedRegions.insert(region);
                //cout << "shortest path: " << testingTile->point << ", " << otherTile->point << " from region " << region->regionName << " to region " << otherRegion->regionName <<"\n";
                //Point topLeft = getTopLeft(testingTile, otherTile);
                //Point bottomRight = getBottomRight(testingTile, otherTile);
                //
                //size_t height = bottomRight.y - topLeft.y + 1;
                //size_t width = bottomRight.x - topLeft.x + 1;
                //vector<vector<size_t>> scores = countTileSteps(height, width);
                //for(size_t i=0; i<height; i++) {
                //    for(size_t j=0; j<width; j++) {
                //        if(tileMap[topLeft.y+i][topLeft.x+j]->type != '#') { continue; }
                //        tileMap[topLeft.y+i][topLeft.x+j]->score += scores[i][j];
                //    }
                //}
            }
        }
    }
}


bool Map::isUsefulPath(Region* region, Region* otherRegion, Tile* tile1, Tile* tile2) {
    Point topLeft = getTopLeft(tile1, tile2);
    Point bottomRight = getBottomRight(tile1, tile2);
    vector<Tile*> pathBox = createTileBox(topLeft, bottomRight);
    for(Tile* tile : pathBox) {
        if(tile->type == '~') {
            return false;
        }
        if(getRegion(tile) != NULL && getRegion(tile) != region && getRegion(tile) != otherRegion) {
            return false;
        }
    }
    return true;
}

void Map::initializeTripletConnections() {
    vector<Region*> regionsVector;
    for(auto it : regions) {
        regionsVector.push_back(it.second);
    }

    unordered_set<Tile*> lockedRegions;
    size_t n = regionsVector.size();
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            for (size_t k = j + 1; k < n; k++) {
                if(!mapDisjointSet.connected(regionsVector[i], regionsVector[j]) || 
                   !mapDisjointSet.connected(regionsVector[j], regionsVector[k]) ||
                   !mapDisjointSet.connected(regionsVector[i], regionsVector[k])) {
                    continue;
                }
                vector<Region*> triplet = {regionsVector[i], regionsVector[j], regionsVector[k]};
                for(Tile* optimalTile : calculateOptimalTriplet(triplet)) {
                    lockedRegions.insert(optimalTile);
                }
            }
        }
    }
    vector<vector<char>> testMap(height, vector<char>(length, '.'));
    for(Tile* tile : lockedRegions) {
        testMap[tile->point.y][tile->point.x] = 'X';
    }
    for(vector<char> row : testMap) {
        for(char tileText : row) {
            cout << tileText;
        }
        cout << "\n";
    }
    cout << "\n";
}


unordered_set<Tile*> Map::calculateOptimalTriplet(vector<Region*> triplet) {
    Region* region1 = triplet[0];
    Region* region2 = triplet[1];
    Region* region3 = triplet[2];


    //Region::Connection* optimalConnection = NULL;
    //optimalTiles should store all lockedRegion tiles
    unordered_set<Tile*> optimalTiles;
    size_t optimalDistance = 999999;

    //region1 <--> region2
    for(Region::Connection* connection : region1->connections) {
        if(connection->getOther(region1) == region2) {
            Point topLeft = getTopLeft(connection->tile1, connection->tile2);
            Point bottomRight = getBottomRight(connection->tile1, connection->tile2);
            vector<Tile*> tileBox = createTileBox(topLeft, bottomRight);
            for(Tile* tile : tileBox) {
                /**
                 * First probe:  left, -x,  0
                 * Second probe: up,    0, -y
                 * Third probe:  right, x,  0
                 * Fourth probe: down,  0,  y
                 */
                //if(tile->point.x == 10 && tile->point.y == 3) {
                //    cout << "testing tile " << tile->point << "\n";
                //}
                for(size_t i=0; i<4; i++) {
                    size_t distance = 0;
                    int xInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
                    int yInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
                    bool reachesThirdRegion = tryReachThirdRegion(distance, tile, xInc, yInc, region1, region2, region3);
                    if(reachesThirdRegion && distance < optimalDistance) {
                        //cout << "new opt path from " << tile->point << " between " << region1->regionName << region2->regionName << region3->regionName<<"\n";
                        optimalTiles.clear();
                        optimalTiles.insert(tile);
                        optimalDistance = distance;
                    }
                    else if(reachesThirdRegion && distance == optimalDistance) {
                        //cout << "huh " << distance << " " << tile->point << "\n";
                        optimalTiles.insert(tile);
                    }
                }
            }
        }
    }
    //region1 <--> region3
    for(Region::Connection* connection : region1->connections) {
        if(connection->getOther(region1) == region3) {
            Point topLeft = getTopLeft(connection->tile1, connection->tile2);
            Point bottomRight = getBottomRight(connection->tile1, connection->tile2);
            vector<Tile*> tileBox = createTileBox(topLeft, bottomRight);
            for(Tile* tile : tileBox) {
                /**
                 * First probe:  left, -x,  0
                 * Second probe: up,    0, -y
                 * Third probe:  right, x,  0
                 * Fourth probe: down,  0,  y
                 */
                for(size_t i=0; i<4; i++) {
                    size_t distance = 0;
                    int xInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
                    int yInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
                    bool reachesThirdRegion = tryReachThirdRegion(distance, tile, xInc, yInc, region1, region3, region2);
                    if(reachesThirdRegion && distance < optimalDistance) {
                        optimalTiles.clear();
                        optimalTiles.insert(tile);
                        optimalDistance = distance;
                    }
                    else if(reachesThirdRegion && distance == optimalDistance) {
                        optimalTiles.insert(tile);
                    }
                }
            }
        }
    }
    //region2 <--> region3
    for(Region::Connection* connection : region2->connections) {
        if(connection->getOther(region2) == region3) {
            Point topLeft = getTopLeft(connection->tile1, connection->tile2);
            Point bottomRight = getBottomRight(connection->tile1, connection->tile2);
            vector<Tile*> tileBox = createTileBox(topLeft, bottomRight);
            for(Tile* tile : tileBox) {
                /**
                 * First probe:  left, -x,  0
                 * Second probe: up,    0, -y
                 * Third probe:  right, x,  0
                 * Fourth probe: down,  0,  y
                 */
                for(size_t i=0; i<4; i++) {
                    size_t distance = 0;
                    int xInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
                    int yInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
                    bool reachesThirdRegion = tryReachThirdRegion(distance, tile, xInc, yInc, region2, region3, region1);
                    if(reachesThirdRegion && distance < optimalDistance) {
                        optimalTiles.clear();
                        optimalTiles.insert(tile);
                        optimalDistance = distance;
                    }
                    else if(reachesThirdRegion && distance == optimalDistance) {
                        optimalTiles.insert(tile);
                    }
                }
            }
        }
    }

    //for(Tile* tile : optimalTiles) {
    //    std::cout << "lockedRegion tile: " << tile->point << " with dist " << optimalDistance << " connecting regions "
    //              << region1->regionName << region2->regionName << region3->regionName<<"\n";
    //}
    return optimalTiles;
}




bool Map::tryReachThirdRegion(size_t& returnDistance, Tile* startingTile, int xInc, int yInc, Region* region1, Region* region2, Region* region3) {
    returnDistance = 0;
    Point nextPoint = startingTile->point;
    while(true) {
        //This finds if the nextTile is a perimeter of region3
        //We test the point itself as well because it could be a perimeter of region3
        if(!isPointValid(nextPoint)) { return false; }
        Tile* nextTile = getTile(nextPoint);
        if(nextTile->type == '~') { return false; }
        if(getRegion(regionalDisjointSet.find(nextTile)) != NULL && 
          (getRegion(regionalDisjointSet.find(nextTile)) != region1 || getRegion(regionalDisjointSet.find(nextTile)) != region2 || getRegion(regionalDisjointSet.find(nextTile)) != region3)) { return false; }
        else if(region3->perimeter.find(nextTile) != region3->perimeter.end()) {
            return true;
        }
        //else, we go to the next tile
        returnDistance++;
        nextPoint.x += xInc;
        nextPoint.y += yInc;
    }
}





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



vector<vector<size_t>> Map::countTileSteps(size_t m, size_t n) {
    // Initialize a DP table to store the number of ways to reach each tile
    vector<vector<size_t>> dp(m, vector<size_t>(n, 0));
    
    // Initialize a step count table to store the number of times each tile is stepped on
    vector<vector<size_t>> stepCount(m, vector<size_t>(n, 0));
    
    // Base case: There's only one way to reach the starting tile (0, 0)
    dp[0][0] = 1;
    
    // Fill the DP table
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i > 0) {
                dp[i][j] += dp[i-1][j]; // Move down
            }
            if (j > 0) {
                dp[i][j] += dp[i][j-1]; // Move right
            }
        }
    }
    
    // Calculate the number of times each tile is stepped on
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            // Number of ways to reach (i, j)
            size_t waysToReach = dp[i][j];
            // Number of ways to go from (i, j) to (m-1, n-1)
            size_t waysToEnd = dp[m-1-i][n-1-j];
            // Total steps on (i, j) is waysToReach * waysToEnd
            stepCount[i][j] = waysToReach * waysToEnd;
        }
    }
    
    return stepCount;
}

Point Map::getTopLeft(Tile* tile1, Tile* tile2) {
    return Point(tile1->point.x < tile2->point.x ? tile1->point.x : tile2->point.x,
                 tile1->point.y < tile2->point.y ? tile1->point.y : tile2->point.y);
}


Point Map::getBottomRight(Tile* tile1, Tile* tile2) {
    return Point(tile1->point.x > tile2->point.x ? tile1->point.x : tile2->point.x,
                 tile1->point.y > tile2->point.y ? tile1->point.y : tile2->point.y);
}

vector<Tile*> Map::createTileBox(Point topLeft, Point bottomRight) {
    vector<Tile*> output;
    for(int i=topLeft.y; i<=bottomRight.y; i++) {
        for(int j=topLeft.x; j<=bottomRight.x; j++) {
            output.push_back(tileMap[i][j]);
        }
    }
    return output;
}