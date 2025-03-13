#ifndef TILE_H
#define TILE_H

#include <iostream>
#include "Point.h"

struct Tile {
    char type;
    Point point;
    bool isPerimeter = false;
    /**
     * neighbors[0] = top neighbor
     * neighbors[1] = right neighbor
     * neighbors[2] = bottom neighbor
     * neighbors[3] = left neighbor
     * NULL = edge of map
     */
    Tile* neighbors[4] = {NULL};

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

    void setIsPerimeter() {
        isPerimeter = true;
    }
};

#endif
