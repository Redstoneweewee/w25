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
    /**Only used in initializing region connections*/
    Tile* parentTile;
    /**Perimeter tiles are wall tiles touching the border of the region 
     * but is not actually a part of the region's disjoint set*/
    std::unordered_set<Tile*> perimeter = std::unordered_set<Tile*>{};
    std::vector<Tile*> bombs = std::vector<Tile*>{};
    std::unordered_set<Connection*> connections;
    std::unordered_set<Region*> connectedRegions;
    private:
    bool mIsLockedRegion = false;
    bool mIsLocked = false;
    public:
    ~Region() {
        for (Connection* c : connections) {
            c->getOther(this)->connections.erase(c);
            delete c;
        }
    }

    void setLockedRegion(bool lock) {
        mIsLockedRegion = lock;
        mIsLocked = lock;
    }
    bool isLockedRegion() {
        return mIsLockedRegion;
    }
    bool isLocked() {
        return mIsLocked;
    }
    void lock() {
        mIsLocked = true;
    }
    void unlock() {
        mIsLocked = false;
    }
};

#endif
