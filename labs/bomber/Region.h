#ifndef REGION_H
#define REGION_H

#include <vector>
#include <unordered_set>
#include "Tile.h"

struct Connection {
    size_t weight;
    Region* region1;
    Region* region2;

    Region* getOther(const Region* current) {
        if(current == region1) {
            return region2;
        }
        return region2;
    }
};

struct Region {
    /** The parent tile in the disjoint set */
    Tile* parentTile;
    std::vector<Tile*> perimeter;
    std::vector<Tile*> bombs;
    std::vector<Connection> connections;
};

#endif
