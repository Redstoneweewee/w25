#include "Map.h"
#include <sstream>
#include "Errors.h"

// Member Functions
Map::Map(std::istream& stream) {
    height = 0;

    string currentLine;
    length = currentLine.size();

    while (getline(stream, currentLine, '\n')) {
        dataMap.push_back(vector<char>{});
        for (char c : currentLine) {
            dataMap[height].push_back(c);
        }
        height++;
    }
}


std::string Map::route(Point src, Point dst) {
    if (!isPointValid(src, true) || !isPointValid(dst, false)) {
        throw PointError(src);
    }
    return "";
}


void Map::printMap() const {
    for(vector<char> row : dataMap) {
        for(char tile: row) {
            std::cout << tile;
        }
        std::cout << "\n";
    }
}




bool Map::isPointValid(Point p, bool isStartingPoint) const {
    if(p.x < 0 || p.y < 0) { return false; }
    if(p.x >= (int)length || p.y >= (int)height) { return false; }
    if(dataMap[p.x][p.y] == '~') { return false; }
    if(isStartingPoint && dataMap[p.x][p.y] == '#') { return false; }
    return true;
}