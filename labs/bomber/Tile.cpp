#include "Tile.h"

void Tile::setNeighbors(Tile* neighbors[4]) {
    for(size_t i=0; i<4; i++) {
        this->neighbors[i] = neighbors[i];
    }
}