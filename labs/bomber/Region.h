#ifndef REGION_H
#define REGION_H

#include <vector>
#include <unordered_set>
#include "Tile.h"


struct Region {
    struct Connection {
        size_t weight = 0;
        Region* region1 = NULL;
        Tile*   tile1   = NULL;
        Region* region2 = NULL;
        Tile*   tile2   = NULL;

        Region* getOther(const Region* current) {
            if(current == region1) {
                return region2;
            }
            return region1;
        }
        Tile* getSelfTile(const Region* current) {
            if(current == region1) {
                return tile1;
            }
            return tile2;
        }
        Tile* getOtherTile(const Region* current) {
            if(current == region1) {
                return tile2;
            }
            return tile1;
        }
    };
    /**Mainly for testing purporses, not actually necessary */
    char regionName = ' ';
    std::vector<Tile*> perimeter = std::vector<Tile*>{};
    std::vector<Tile*> bombs = std::vector<Tile*>{};
    std::unordered_set<Connection*> connections;
    std::unordered_set<Region*> connectedRegions;

    ~Region() {
        for(Connection* c : connections) {
            c->getOther(this)->connections.erase(c);
            delete c;
        }
    }
};

#endif
