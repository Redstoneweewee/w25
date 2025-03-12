#ifndef TILE_H
#define TILE_H

#include <iostream>
#include "Point.h"

struct Tile {
    char type;
    Point point;
    Tile* neighbors[4];

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

    void setNeighbors(Tile* neighbors[4]);
};

#endif
