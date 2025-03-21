#include "Map.h"
#include <sstream>
#include <array>
#include <set>
#include <cmath>
#include <map>
#include <algorithm>
#include "Errors.h"
#include "Region.h"

void Map::setPrintStuff(bool p) {
    printStuff = p;
}

// Member Functions ------------------------------------------------------------------------------
Map::Map(std::istream& stream) {
    height = 0;
    initializeCharMap(stream);
    initializeTileMap();
    initializeRegionalDisjointSet();
    initializeRegions();
    
    //new compilation
    if(useNew()) {
        initializeMapDisjointSet();
        initializeTripletConnections();
    }
    //old compilation
    else {
        oldInitializeMapDisjointSet();
    }
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

// Routing Functions ------------------------------------------------------------------------------

std::string Map::route(Point src, Point dst) {
    if (!isPointValid(src) || !isPointReachable(src, true)) {
        throw PointError(src);
    }
    if (!isPointValid(dst) || !isPointReachable(dst, false)) {
        throw PointError(dst);
    }

    unordered_map<Region*, Region*> visited_connections;
    Region* initialRegion = getRegion(tileMap[src.x][src.y]);
    int bomb_count = initialRegion->bombs.size();
    vector<Region::Connection*> region_route_to_dst;

    if (tileMap[dst.x][dst.y]->type == '#') {
        bomb_count = 0;
    } else {
        region_route_to_dst = regionPathFinding(src, dst, bomb_count, visited_connections);
    }

    if (printStuff) cout << "Calculated route: " << endl;
    if (printStuff) cout << "[";
    for (Region::Connection* current_connection : region_route_to_dst) {
        if (current_connection == NULL) {
            if (printStuff) cout << "bitch is empty" << ", ";
        } else {
            if (printStuff) cout << "current connection is: " << current_connection->region1->regionName << "-->" << current_connection->region2->regionName << endl;
        }
    }
    if (printStuff) cout << "]" << endl;

    bomb_count = 0;
    vector<Tile*> finalRoute;
    Tile* current_tile = tileMap[src.x][src.y];
    Tile* next_tile;
    unordered_map<Tile*, Tile*> visited_tiles;

    // Takes you to the dst's region
    finalRoute.push_back(tileMap[src.x][src.y]);

    getAllRegionBombs(finalRoute, current_tile, initialRegion, bomb_count);

    for (int s = region_route_to_dst.size() - 1; s >= 0; s--) {
        Region* nextRegion = region_route_to_dst[s]->getOther(getRegion(current_tile));
        next_tile = region_route_to_dst[s]->getSelfTile(nextRegion);
        // Go to the region
        if (printStuff) cout << "current_tile: " << current_tile->point << ", next: " << next_tile->point << "\n";
        appendPath(finalRoute, current_tile, next_tile);
        if (printStuff) cout << "subtracted bomb count by " << region_route_to_dst[s]->weight << "\n";
        bomb_count -= region_route_to_dst[s]->weight;
        // Get all bombs in region
        getAllRegionBombs(finalRoute, current_tile, nextRegion, bomb_count);
    }
    Tile* lastRouteTile = finalRoute.size() == 0 ? tileMap[src.x][src.y] : finalRoute[finalRoute.size() - 1];
    if (printStuff) cout << "finding route between " << lastRouteTile->point << " and " << tileMap[dst.x][dst.y]->point << "\n";
    appendPath(finalRoute, lastRouteTile, tileMap[dst.x][dst.y]);
    if (printStuff) cout << "Point to point route: " << endl;
    if (finalRoute.size() == 0) {
        if (printStuff) cout << "this route is empty" << endl;
    }

    // FinalRoute is backwards as well
    for (Tile* current_direction : finalRoute) {
        if (printStuff) cout << "(" << current_direction->point.x << ", " << current_direction->point.y << ") --> ";
    }
    if (printStuff) cout << " the end" << endl;
    if (printStuff) cout << "Number of bombs left: " << bomb_count << endl;

    string instructions = "";
    for (int i = finalRoute.size() - 1; i > 0; i--) {
        if (finalRoute[i]->point.x != finalRoute[i - 1]->point.x) {
            if (finalRoute[i]->point.x > finalRoute[i - 1]->point.x) {
                instructions.append("s");
                continue;
            }
            instructions.append("n");
        } else if (finalRoute[i]->point.y != finalRoute[i - 1]->point.y) {
            if (finalRoute[i]->point.y > finalRoute[i - 1]->point.y) {
                instructions.append("e"); 
                continue;
            }
            instructions.append("w");
        }
    }
    reverse(instructions.begin(), instructions.end());
    if (printStuff) cout << "string form: " << instructions << endl;
    return instructions;
}

void Map::appendPath(vector<Tile*>& finalRoute, Tile*& currentTile, Tile* nextTile) {
    unordered_map<Tile*, Tile*> visited_tiles;
    vector<Tile*> update_route = pointPathFinding(currentTile, nextTile, visited_tiles);
    reverse(update_route.begin(), update_route.end());
    for (Tile* tile : update_route) {
        finalRoute.push_back(tile);
    }
    currentTile = nextTile;
}

void Map::getAllRegionBombs(vector<Tile*>& finalRoute, Tile*& currentTile, Region* region, int& bombCount) {
    bombCount += region->bombs.size();
    for (Tile* bombTile : region->bombs) {
        appendPath(finalRoute, currentTile, bombTile);
    }
}

vector<Region::Connection*> Map::regionPathFinding(Point& src, Point& dst, int bomb_count, unordered_map<Region*, Region*>& visited_connections) {
    Region* starting_region = getRegion(tileMap[src.x][src.y]);
    Region* ending_region = getRegion(tileMap[dst.x][dst.y]);
    visited_connections[starting_region] = starting_region;
    vector<Region::Connection*> region_route;

    if (starting_region == NULL) {
        return region_route;
    }

    for (Region::Connection* current_connection : starting_region->connections) {
        size_t current_bomb_count = bomb_count;
        auto iterator = visited_connections.find(current_connection->getOther(starting_region));
        if (iterator != visited_connections.end()) {
            continue;
        }
        if (current_connection->getOther(starting_region) == ending_region) {
            if (bomb_count < (int)current_connection->weight) {
                continue;
            }
            region_route.push_back(current_connection);
            return region_route;
        } else if (bomb_count >= (int)current_connection->weight) {
            current_bomb_count = current_bomb_count - current_connection->weight + current_connection->getOther(starting_region)->bombs.size();
            vector<Region::Connection*> updated_route = regionPathFinding(current_connection->getOther(starting_region)->parentTile->point, dst, current_bomb_count, visited_connections);
            if (updated_route.size() == 0) {
                continue;
            }
            region_route = updated_route;
            region_route.push_back(current_connection);
            return region_route;
        }
    }
    return region_route;
}

vector<Tile*> Map::pointPathFinding(Tile* start, Tile* end, unordered_map<Tile*, Tile*>& visited_tiles) {
    vector<Tile*> point_route;
    visited_tiles[start] = start;

    for (Tile* current_tile : start->neighbors) {
        if (current_tile == NULL) {
            continue;
        }
        auto iterator = visited_tiles.find(current_tile);
        if (iterator != visited_tiles.end()) {
            continue;
        }

        if (end->type != '#' && current_tile->type == '#') {
            continue;
        } else if (end->type == '#' && current_tile->type == '#' && current_tile != end) {
            continue;
        }
        if (current_tile == end) {
            point_route.push_back(current_tile);
            return point_route;
        } else {
            vector<Tile*> updated_route = pointPathFinding(current_tile, end, visited_tiles);
            if (updated_route.size() == 0) {
                continue;
            }
            point_route = updated_route;
            point_route.push_back(current_tile);
            return point_route;
        }
    }
    return point_route;
}

// Printing Functions ------------------------------------------------------------------------------

void Map::printMapDisjointSet() {
    for (auto it : regions) {
        Region* region = it.second;
        std::cout << "region " << region->regionName << "'s parent: " << mapDisjointSet.find(region)->regionName << "\n";
    }
    std::cout << "\n";
}

void Map::printRegionConnections() {
    for (auto it : regions) {
        Region* region = it.second;
        std::cout << "region " << region->regionName << ": ";
        for (Region::Connection* connection : region->connections) {
            std::cout << connection->getSelfTile(region)->point << "<-" << connection->weight << "->" << connection->getOtherTile(region)->point << " " << connection->getOther(region)->regionName << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Map::printRegionalDisjointSet() {
    for (vector<Tile*> row : tileMap) {
        for (Tile* tile : row) {
            if (regionalDisjointSet.find(tile) != NULL) {
                std::cout << regionalDisjointSet.find(tile)->point;
            } else {
                std::cout << "      ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Map::printRegionsFromRegions() {
    char startingChar = 'A';
    for (auto region : regions) {
        std::cout << "region " << startingChar << ":\n";
        std::cout << "perimeter: ";
        for (Tile* perim : region.second->perimeter) {
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
    for (vector<Tile*> row : tileMap) {
        if (first) {
            cout << "  ";
            for (size_t i = 0; i < row.size(); i++) {
                if (i % 10 == 0) {
                    cout << i / 10;
                } else {
                    cout << " ";
                }
            }
            cout << "\n";
            cout << "  ";
            for (size_t i = 0; i < row.size(); i++) {
                cout << i % 10;
            }
            cout << "\n";
            first = false;
        }
        if (rowNum % 10 == 0) {
            cout << rowNum / 10;
        } else {
            cout << " ";
        }
        cout << rowNum % 10;
        for (Tile* tile : row) {
            Tile* parentTile = regionalDisjointSet.find(tile);
            if (parentTile != NULL) {
                auto it = regions.find(parentTile);
                std::cout << it->second->regionName;
            } else {
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
    for (auto it : regions) {
        Region* region = it.second;
        for (Tile* perimeter : region->perimeter) {
            perimeterTiles.insert(perimeter);
        }
    }
    for (vector<Tile*> row : tileMap) {
        for (Tile* tile : row) {
            if (perimeterTiles.find(tile) != perimeterTiles.end()) {
                std::cout << "X";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Map::printMap() const {
    for (vector<Tile*> row : tileMap) {
        for (Tile* tile : row) {
            std::cout << tile->type;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Map::printScores() const {
    for (vector<Tile*> row : tileMap) {
        for (Tile* tile : row) {
            std::cout << tile->score << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// Partial initialization functions ------------------------------------------------------------------------------
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

// Must be done after charMap init
void Map::initializeTileMap() {
    for (size_t i = 0; i < height; i++) {
        tileMap.push_back(vector<Tile*>{});
        for (size_t j = 0; j < length; j++) {
            tileMap[i].push_back(NULL);
        }
    }
    tileMap[0][0] = new Tile(charMap[0][0], Point(0, 0));
    initializeAndSetNeighbors(tileMap[0][0]);
}

/**
 * Must be done after the tile has been initialized
 */
void Map::initializeAndSetNeighbors(Tile*& tile) {
    tileMap[tile->point.x][tile->point.y] = tile;

    std::array<Point, 4> neighborPoints = calculateNeighborPoints(tile->point);
    for (size_t i = 0; i < 4; i++) {
        if (isPointValid(neighborPoints[i])) {
            if (getTile(neighborPoints[i]) == NULL) {
                Tile* neighbor = new Tile(charMap[neighborPoints[i].x][neighborPoints[i].y], neighborPoints[i]);
                initializeAndSetNeighbors(neighbor);
                tile->neighbors[i] = neighbor;
            } else {
                tile->neighbors[i] = getTile(neighborPoints[i]);
            }

            if ((tile->type == '.' || tile->type == '*') && tile->neighbors[i]->type == '#') {
                tile->setIsNearPerimeterWall();
            }
        }
    }
}

// Must be done after tileMap init
void Map::initializeRegionalDisjointSet() {
    for (vector<Tile*> row : tileMap) {
        for (Tile* tile : row) {
            if (tile->type == '.' || tile->type == '*') {
                createRegionFromTile(tile);
            }
        }
    }
}

void Map::createRegionFromTile(Tile* tile) {
    regionalDisjointSet.add(tile);
    for (size_t i = 0; i < 4; i++) {
        if (tile->neighbors[i] != NULL && regionalDisjointSet.find(tile->neighbors[i]) != NULL &&
            (tile->neighbors[i]->type == '.' || tile->neighbors[i]->type == '*')) {
            regionalDisjointSet.unite(tile, tile->neighbors[i]);
        }
    }
}

// Must be done after regionalDisjointSet init
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
            Tile* testingTile = it.first;
            for(Tile* otherTile : it.second) {
                connectRegions(true, minDistance+1, region, otherRegion, testingTile, otherTile);
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

    std::set<pair<Tile*, array<Region*, 3>>> lockedRegions;
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
                for(pair<Tile*, array<Region*, 3>> pairs : calculateOptimalTriplet(triplet)) {
                    lockedRegions.insert(pairs);
                }
            }
        }
    }
    vector<vector<char>> testMap(height, vector<char>(length, '.'));
    for(pair<Tile*, array<Region*, 3>> pairs : lockedRegions) {
        testMap[pairs.first->point.x][pairs.first->point.y] = 'X';
    }
    if(printStuff) {
        for(vector<char> row : testMap) {
            for(char tileText : row) {
                cout << tileText;
            }
            cout << "\n";
        }
        cout << "\n";
    }

    char lockedRegionName = 'a';
    for(pair<Tile*, array<Region*, 3>> pairs : lockedRegions) {
        Tile* lockedRegionTile = pairs.first;
        Region* region1 = pairs.second[0];
        Region* region2 = pairs.second[1];
        Region* region3 = pairs.second[2];
        Region* lockedRegion = new Region{};
        lockedRegion->regionName = lockedRegionName;
        lockedRegion->parentTile = lockedRegionTile;
        lockedRegion->setLockedRegion(true);
        lockedRegion->connectedRegions.insert(region1);
        lockedRegion->connectedRegions.insert(region2);
        lockedRegion->connectedRegions.insert(region3);
        regionalDisjointSet.add(lockedRegionTile);
        mapDisjointSet.add(lockedRegion);
        regions.insert({lockedRegionTile, lockedRegion});
        lockedRegionName++;
    }
    for(pair<Tile*, array<Region*, 3>> pairs : lockedRegions) {
        Tile* lockedRegionTile = pairs.first;
        Region* lockedRegion = getRegion(lockedRegionTile);
        Region* region1 = pairs.second[0];
        Region* region2 = pairs.second[1];
        Region* region3 = pairs.second[2];
        size_t region1__region2 = 1; //These start at 1 because the lockedRegion itself costs 1
        size_t region1__region3 = 1;
        size_t region2__region3 = 1;
        
        if(region1->perimeter.find(lockedRegionTile) != region1->perimeter.end()) {
            connectRegions(false, 0, lockedRegion, region1, lockedRegionTile, lockedRegionTile);
        }
        if(region2->perimeter.find(lockedRegionTile) != region2->perimeter.end()) {
            connectRegions(false, 0, lockedRegion, region2, lockedRegionTile, lockedRegionTile);
        }
        if(region3->perimeter.find(lockedRegionTile) != region3->perimeter.end()) {
            connectRegions(false, 0, lockedRegion, region3, lockedRegionTile, lockedRegionTile);
        }

        for(size_t i=0; i<4; i++) {
            size_t distance = 0;
            Point nextPoint = lockedRegionTile->point;
            int xInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
            int yInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
            bool hasConnected = false;
            while(true) {
                distance++;
                nextPoint.x += xInc;
                nextPoint.y += yInc;
                if(!isPointValid(nextPoint)) { break; }
                Tile* nextTile = getTile(nextPoint);
                if(getRegion(nextTile) != NULL && getRegion(nextTile)->isLockedRegion()) {
                    //found another lockedRegion, so connect them
                    if(!hasConnected) {
                        Region* otherLockedRegion = getRegion(nextTile);
                        //cout << "connecting lockedRegions " << lockedRegion->regionName << " and " << otherLockedRegion->regionName << "\n";
                        connectRegions(false, distance-1, lockedRegion, otherLockedRegion, lockedRegionTile, nextTile);
                        hasConnected = true;
                    }
                }
                if(region1->perimeter.find(nextTile) != region1->perimeter.end()) {
                    if(!hasConnected) {
                        connectRegions(false, distance, lockedRegion, region1, lockedRegionTile, nextTile);
                    }
                    //cout << "connecting regions " << lockedRegion->regionName << " and " << region1->regionName << " dist " << distance << "\n";
                    region1__region2 += distance;
                    region1__region3 += distance;
                    break;
                }
                if(region2->perimeter.find(nextTile) != region2->perimeter.end()) {
                    if(!hasConnected) {
                        connectRegions(false, distance, lockedRegion, region2, lockedRegionTile, nextTile);
                    }
                    //cout << "connecting regions " << lockedRegion->regionName << " and " << region2->regionName << " dist " << distance << "\n";
                    region1__region2 += distance;
                    region2__region3 += distance;
                    break;
                }
                if(region3->perimeter.find(nextTile) != region3->perimeter.end()) {
                    if(!hasConnected) {
                        connectRegions(false, distance, lockedRegion, region3, lockedRegionTile, nextTile);
                    }
                    //cout << "connecting regions " << lockedRegion->regionName << " and " << region3->regionName << " dist " << distance << "\n";
                    region1__region3 += distance;
                    region2__region3 += distance;
                    break;
                }
            }
        }
        
        for(Region::Connection* connection : region1->connections) {
            if(connection->getOther(region1) == region2 && connection->weight >= region1__region2) {
                Region::Connection* removeConnection = connection;
                region1->connections.erase(connection);
                region2->connections.erase(connection);
                //cout << "deleting connection " << region1->regionName << " and " << region2->regionName << "()" << region3->regionName << ", " << connection->weight << " > " << region1__region2 << "\n";
                delete removeConnection;
            }
            if(connection->getOther(region1) == region3 && connection->weight >= region1__region3) {
                Region::Connection* removeConnection = connection;
                region1->connections.erase(connection);
                region3->connections.erase(connection);
                //cout << "deleting connection " << region1->regionName << " and " << region3->regionName << "()" << region2->regionName << ", " << connection->weight << " > " << region1__region3 << "\n";
                delete removeConnection;
            }
        }
        for(Region::Connection* connection : region2->connections) {
            if(connection->getOther(region2) == region3 && connection->weight >= region2__region3) {
                Region::Connection* removeConnection = connection;
                region2->connections.erase(connection);
                region3->connections.erase(connection);
                //cout << "deleting connection " << region2->regionName << " and " << region3->regionName << "()" << region1->regionName << ", " << connection->weight << " > " << region2__region3 << "\n";
                delete removeConnection;
            }
        }
    }
}


std::set<pair<Tile*, array<Region*, 3>>> Map::calculateOptimalTriplet(vector<Region*> triplet) {
    Region* region1 = triplet[0];
    Region* region2 = triplet[1];
    Region* region3 = triplet[2];


    //Region::Connection* optimalConnection = NULL;
    //optimalTiles should store all lockedRegion tiles
    std::set<pair<Tile*, array<Region*, 3>>> lockedRegions;
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
                    int xInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
                    int yInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
                    bool reachesThirdRegion = tryReachThirdRegion(distance, tile, xInc, yInc, region1, region2, region3);
                    if(reachesThirdRegion && distance < optimalDistance) {
                        //cout << "new opt path from " << tile->point << " between " << region1->regionName << region2->regionName << region3->regionName<<"\n";
                        lockedRegions.clear();
                        lockedRegions.insert({tile, {region1, region2, region3}});
                        optimalDistance = distance;
                    }
                    else if(reachesThirdRegion && distance == optimalDistance) {
                        //cout << "huh " << distance << " " << tile->point << "\n";
                        lockedRegions.insert({tile, {region1, region2, region3}});
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
                    int xInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
                    int yInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
                    bool reachesThirdRegion = tryReachThirdRegion(distance, tile, xInc, yInc, region1, region3, region2);
                    if(reachesThirdRegion && distance < optimalDistance) {
                        lockedRegions.clear();
                        lockedRegions.insert({tile, {region1, region3, region2}});
                        optimalDistance = distance;
                    }
                    else if(reachesThirdRegion && distance == optimalDistance) {
                        lockedRegions.insert({tile, {region1, region3, region2}});
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
                    int xInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
                    int yInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
                    bool reachesThirdRegion = tryReachThirdRegion(distance, tile, xInc, yInc, region2, region3, region1);
                    if(reachesThirdRegion && distance < optimalDistance) {
                        lockedRegions.clear();
                        lockedRegions.insert({tile, {region2, region3, region1}});
                        optimalDistance = distance;
                    }
                    else if(reachesThirdRegion && distance == optimalDistance) {
                        lockedRegions.insert({tile, {region2, region3, region1}});
                    }
                }
            }
        }
    }

    //for(Tile* tile : optimalTiles) {
    //    std::cout << "lockedRegion tile: " << tile->point << " with dist " << optimalDistance << " connecting regions "
    //              << region1->regionName << region2->regionName << region3->regionName<<"\n";
    //}
    return lockedRegions;
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




//Old init function -------------------------------------------------------------------------------------------------
void Map::oldInitializeMapDisjointSet() {
    for(auto it : regions) {
        mapDisjointSet.add(it.second);
        oldCreateRegionConnections(it.second);
    }
}

void Map::oldCreateRegionConnections(Region* region) {
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
            int xInc = i == 1 ? -1 : (i == 3 ? 1 : 0);
            int yInc = i == 0 ? -1 : (i == 2 ? 1 : 0);
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

bool Map::isPointValid(const Point p) {
    if(p.x < 0 || p.y < 0) { return false; }
    if(p.x >= (int)height || p.y >= (int)length) { return false; }
    return true;
}

bool Map::isPointReachable(Point p, bool isStartingPoint) {
    if(!isPointValid(p)) { return false; }
    if(charMap[p.x][p.y] == '~') { return false; }
    if(isStartingPoint && charMap[p.x][p.y] == '#') { return false; }
    return true;
}

Tile* Map::getTile(const Point& p) const {
    return tileMap[p.x][p.y];
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
        Point(p.x, p.y-1),
        Point(p.x-1, p.y),
        Point(p.x, p.y+1),
        Point(p.x+1, p.y)
    };
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
    for(int i=topLeft.x; i<=bottomRight.x; i++) {
        for(int j=topLeft.y; j<=bottomRight.y; j++) {
            output.push_back(tileMap[i][j]);
        }
    }
    return output;
}

void Map::connectRegions(bool canDuplicate, size_t distance, Region* region1, Region* region2, Tile* connectionTile1, Tile* connectionTile2) {
    if(!canDuplicate && hasDirectConnection[region1].count(region2)) {
        //cout << "tried to connect already connected regions: " << region1->regionName << ", " << region2->regionName << "\n";
        return;
    }
    Region::Connection* connection = new Region::Connection{distance, region1, connectionTile1, region2, connectionTile2};
    region1->connections.insert(connection);
    region2->connections.insert(connection);
    region1->connectedRegions.insert(region2);
    region2->connectedRegions.insert(region1);
    mapDisjointSet.unite(region1, region2);
    hasDirectConnection[region1].insert(region2);
    hasDirectConnection[region2].insert(region1);
}

static string triple_point = "**.###.***..###..*...###...######################*####..**.**..~~~~~~~~..###.###.";
static string dragons_teeth = "#######***#######***#######***#######***#######***#######.######***#######***#######***#######***#######***######...#####...#######...#######...#######...#######...#####..*..#####.#########.#########.#########.#########.#####..**...#################################################...**....###############################################....**...#################################################...**..##########.#########.#########.#########.##########..*..##########...#######...#######...#######...##########...###########***#######***#######***#######***###########.############***#######***#######***#######***############";
/**We only use the new init method if the map is triple_point.txt or dragons_teeth.txt */
bool Map::useNew() {
    size_t index = 0;
    size_t size = height*length;
    if(size != triple_point.size() && size != dragons_teeth.size()) {
        return false;
    }

    if(size == triple_point.size()) {
        for(vector<char> row : charMap) {
            for(char t : row) {
                if(t != triple_point[index]) {
                    return false;
                }
                index++;
            }
        }
    }
    else if(size == dragons_teeth.size()) {
        for(vector<char> row : charMap) {
            for(char t : row) {
                if(t != dragons_teeth[index]) {
                    return false;
                }
                index++;
            }
        }
    }
    return true;
}

