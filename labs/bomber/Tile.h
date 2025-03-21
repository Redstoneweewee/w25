#ifndef TILE_H
#define TILE_H

#include <iostream>
#include "Point.h"

struct Tile {
    char type;
    Point point;
    bool isNearPerimeterWall = false;
    /**
     * neighbors[0] = left neighbor
     * neighbors[2] = top neighbor
     * neighbors[3] = right neighbor
     * neighbors[4] = bottom neighbor
     * NULL = edge of map
     */
    Tile* neighbors[4] = {NULL};
    size_t score = 0;

    Tile(char type, Point point) {
        this->type = type;
        this->point = point;
    }

    Tile(const Tile& other) {
        this->point = other.point;
        this->type = other.type;
        for(size_t i=0; i<4; i++) {
            this->neighbors[i] = other.neighbors[i];
        }
    }

    void setIsNearPerimeterWall() {
        isNearPerimeterWall = true;
    }
};

#endif
