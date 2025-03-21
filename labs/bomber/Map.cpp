#include "Map.h"
#include <sstream>
#include <array>
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

//note: vector with region route to dst is backwars, last region first.
//note: same goes for the point path finding
std::string Map::route(Point& src, Point& dst) {
    //cout << "entered dumb function:" << endl;
    if (!isPointReachable(src, true)) {
        throw PointError(src);
    }
    if (!isPointReachable(dst, false)){
        throw PointError(dst);
    }


    //cout << "confirmed valid point:" << endl;

    unordered_map<Region*, Region*> visited_connections;  //int is no of bombs needed to get to region
    size_t bomb_count = getRegion(tileMap[src.y][src.x])->bombs.size();
    vector<Region::Connection*> region_route_to_dst;

    if (tileMap[dst.y][dst.x]->type == '#'){
        //cout << "detected:" << endl;
        //placement holder code block, so that program doesn't crash
        //when dst is a wall (serious problem that needs to be fized)
        bomb_count = 0;
    }
    else{
        region_route_to_dst = regionPathFinding(src, dst,bomb_count,visited_connections);
    }
    cout << "Calculated route: " << endl;
    cout << "[";
    for (Region::Connection* current_connection: region_route_to_dst){
        if (current_connection == NULL)
        {
            cout << "bitch is empty" << ", ";
        }
        else{
            cout << "current connection is: " << current_connection->region1->regionName << "-->" << current_connection->region2->regionName << endl;
        }
    }
    cout << "]" << endl;

    bomb_count = 0;
    vector<Tile*> finalRoute;
    Tile* current_tile = tileMap[src.y][src.x];
    Tile* next_tile;
    unordered_map<Tile*, Tile*> visited_tiles;

    //takes you to the dst's region
    for (int s = region_route_to_dst.size() -1; s >= 0; s--){
        if (getRegion(current_tile) == region_route_to_dst[s]->region1){
            next_tile = region_route_to_dst[s]->tile2;
        }
        else {
        next_tile = region_route_to_dst[s]->tile1;
        }
    vector<Tile*> update_route = pointPathFinding(current_tile, next_tile, bomb_count, visited_tiles);
        for (Tile* tile: update_route){  //might need to adjust for orientation/order of vector
            finalRoute.push_back(tile);
        }
        bomb_count -= region_route_to_dst[s]->weight;
        current_tile = next_tile;
    }
    finalRoute.push_back(tileMap[src.y][src.x]);
    cout << "Point to point route: " << endl;
    if (finalRoute.size() == 0)
    {
    cout << "this route is empty" << endl;
    }

    //finalRoute is backwards as well
    for (Tile* current_direction: finalRoute){

    cout << "(" << current_direction->point.x << ", " << current_direction->point.y << ") --> ";
    }
    cout << " the end" << endl;
    cout << "Number of bombs left: " << bomb_count << endl;

    string instructions = "";
    for (int i = finalRoute.size() - 1; i > 0; i--){
            if (finalRoute[i]->point.x != finalRoute[i - 1]->point.x){
                if (finalRoute[i]->point.x > finalRoute[i - 1]->point.x){
                    instructions.append("w");
                    continue;
                }
                instructions.append("e");
            }
            else if (finalRoute[i]->point.y != finalRoute[i - 1]->point.y){
                if (finalRoute[i]->point.y > finalRoute[i - 1]->point.y){
                    instructions.append("s");
                    continue;
                }
                instructions.append("n");

            }
    }
    cout << "string form: " << instructions << endl;
    //algorithm to read tile's points (and the changes to get to the next one)
    //to then find out direction of momvement
    // as well as to write a string with appropriate symbol
    return instructions;
}

vector<Region::Connection*> Map::regionPathFinding(Point& src, Point& dst, size_t bomb_count, unordered_map<Region*, Region*>& visited_connections){
    //cout << "Entered region pathfinding" << endl;
    Region* starting_region = getRegion(tileMap[src.y][src.x]);   
    Region* ending_region = getRegion(tileMap[dst.y][dst.x]);
    visited_connections[starting_region] = starting_region;
    vector<Region::Connection*> region_route;

    if (starting_region == NULL){
        return region_route;
    }

    //cout <<"Initalization finished:" << endl;
    //Tile* starting_tile = tileMap[src.y][src.x];
    //Tile* ending_tile = tileMap[dst.y][dst.x];

                //make variables for result from getother()
        for (Region::Connection* current_connection: starting_region->connections){
            //cout << "entered neighbor checking:" << endl;
            //lines of code to check if neighbor region is already in visited (no backtracking allowed)
            size_t current_bomb_count = bomb_count;
            auto iterator = visited_connections.find(current_connection->getOther(starting_region));
            if (iterator != visited_connections.end()){
                //cout << "already checked node:" << endl;
                continue;
            }
            if (current_connection->getOther(starting_region) == ending_region ){
                if (bomb_count < current_connection->weight){
                //cout << "too expensive:" << endl;
                continue; //indicates that this connection couldn't get to dst
                }
                region_route.push_back(current_connection);
            //cout << "found it";
            return region_route;
            } 
            else if (bomb_count >= current_connection->weight){
                current_bomb_count = current_bomb_count - current_connection->weight + current_connection->getOther(starting_region)->bombs.size();
                //visited_connections[starting_region] = starting_region;
                //cout << "checking neighbor's neighbors: " << endl;
                vector<Region::Connection*> updated_route = regionPathFinding(current_connection->getOther(starting_region)->parentTile->point, dst,current_bomb_count, visited_connections);
                    if (updated_route.size() == 0){
                        //cout << "nothing found:" << endl;
                        continue;
                    }
                region_route = updated_route;
                region_route.push_back(current_connection);
                //cout << "recursive recall:" << endl;
                return region_route;
            }
            continue;
        }
    //cout << "found completely nothing:" << endl;
    return region_route;
}

//needs system that first directs the algorithm to find bombs (all of them or amount required)
//and then find the path to dst's region (current systme works but is slow);
vector<Tile*> Map::pointPathFinding(Tile* start, Tile* end, size_t& bomb_count, unordered_map<Tile*, Tile*>& visited_tiles){
    vector<Tile*> point_route;
    visited_tiles[start] = start;

    for (Tile* current_tile: start->neighbors){
        size_t updated_bomb_count = bomb_count;

        if (current_tile == NULL){
            continue;
        }
        //cout << "entered neighbor checking(points):" << endl;
        //lines of code to check if neighbor region is already in visited (no backtracking allowed)
        auto iterator = visited_tiles.find(current_tile);
        if (iterator != visited_tiles.end()){
            //cout << "already checked node:" << endl;
            continue;
        }

        if (current_tile->type == '#'){
            if (bomb_count == 0){
                continue;
            }
            else{
                updated_bomb_count += -1;
            }
        }
        if (current_tile->type == '*'){
            updated_bomb_count += 1;
        }
        if (current_tile == end ){
            point_route.push_back(current_tile);
            //cout << "found it";
            bomb_count = updated_bomb_count;
            return point_route;
        } 
        else{
            //cout << "checking neighbor's neighbors: " << endl;
            vector<Tile*> updated_route = pointPathFinding(current_tile,end,updated_bomb_count, visited_tiles);
                if (updated_route.size() == 0){
                    //cout << "nothing found:" << endl;
                    continue;
                }
            point_route = updated_route;
            point_route.push_back(current_tile);
            bomb_count = updated_bomb_count;
            //cout << "recursive recall:" << endl;
            return point_route;
        }
        continue;
    }
//cout << "found completely nothing:" << endl;
return point_route;
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
                                region->perimeter.push_back(tile->neighbors[i]);
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
            size_t numOfWalls = 1;
            Tile* previousNextTile = testingTile;
            int xInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
            int yInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
            while(true) {
                nextPoint.x += xInc;
                nextPoint.y += yInc;
                if(!isPointValid(nextPoint)) { break; }
                //std::cout << "checking point " << nextPoint << "\n";
                Tile* nextTile = getTile(nextPoint);
                if(nextTile->type == '~') { /*std::cout << "1\n";*/ break; }
                else if(nextTile->type == '#') { 
                    previousNextTile = nextTile;
                    numOfWalls++; 
                } //this has to go before the next statement because walls are not part of the disjoint sets
                else if(regionalDisjointSet.find(nextTile) == region->parentTile) { break; }
                //If none of those are true, then we have reached a new region
                else {
                    Region* otherRegion = getRegion(nextTile);
                    //For optimal path
                    if(region->connectedRegions.find(otherRegion) != region->connectedRegions.end()) {
                        for(Region::Connection* connection : region->connections) {
                            if(connection->getOther(region) == otherRegion && connection->weight > numOfWalls) {
                                if(connection->region1 == region) {
                                    connection->tile1 = testingTile;
                                    connection->tile2 = previousNextTile;
                                }
                                else {
                                    connection->tile1 = previousNextTile;
                                    connection->tile2 = testingTile;
                                }
                                connection->weight = numOfWalls;
                                break;
                            }
                        }
                    }
                    //For new path
                    if(region->connectedRegions.find(otherRegion) == region->connectedRegions.end()) {
                        //std::cout << "3\n";
                        Region::Connection* connection = new Region::Connection{numOfWalls, region, testingTile, otherRegion, previousNextTile};
                        region->connections.insert(connection);
                        otherRegion->connections.insert(connection);
                        region->connectedRegions.insert(otherRegion);
                        otherRegion->connectedRegions.insert(region);
                        mapDisjointSet.add(otherRegion);
                        mapDisjointSet.unite(region, otherRegion);
                    }
                    //If not true, the regions are already connected, so no need to reconnect them
                    break;
                }
            }
        }
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
    if(charMap[p.y][p.x] == '~') { return false; }
    if(isStartingPoint && charMap[p.y][p.x] == '#') { return false; }
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

