#include "Map.h"
#include <sstream>
#include "Errors.h"

// Member Functions
Map::Map(std::istream& stream) {
    height = 0;

    string current_line;
    length = current_line.size();

    // lat and lng might be in the wrong places, haven't check just saying
    while (getline(stream, current_line, '\n')) {
        data_map.push_back(vector<char>{});
        for (char c : current_line) {
            data_map[height].push_back(c);
        }
        height++;
    }
}

/*
std::string Map::route(Point src, Point dst) {
    if (data_map[src.lat][src.lng] == '~' || src.lat >= rows ||
        src.lng >= columns || dst.lat >= rows || dst.lng >= columns) {
        throw PointError(src);
    }
}
*/

void Map::printMap() const {
    for(vector<char> row : data_map) {
        for(char tile: row) {
            std::cout << tile;
        }
        std::cout << "\n";
    }
}