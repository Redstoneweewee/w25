#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <unordered_map>
#include "Tile.h"

class DisjointSet {
private:
    std::unordered_map<Tile*, Tile*> parent; // Maps element -> parent
    std::unordered_map<Tile*, size_t> rank;   // Maps element -> rank

public:
    // Add a new element as its own set
    void add(Tile* tile) {
        if(parent.find(tile) == parent.end()) {
            parent[tile] = tile;
            rank[tile] = 1;
        }
    }

    // Find with path compression
    Tile* find(Tile* tile) {
        if (parent.find(tile) == parent.end()) {
            return NULL;
        }
        if (parent[tile] != tile) {
            parent[tile] = find(parent[tile]);
        }
        return parent[tile];
    }

    // Unites two sets
    void unite(Tile* a, Tile* b) {
        Tile* rootA = find(a);
        Tile* rootB = find(b);

        if (rootA != rootB) {
            // Union by rank
            if (rank[rootA] > rank[rootB]) {
                parent[rootB] = rootA;
            } 
            else if (rank[rootA] < rank[rootB]) {
                parent[rootA] = rootB;
            } 
            else {
                parent[rootB] = rootA;
                rank[rootA]++;
            }
        }
    }

    bool connected(Tile* a, Tile* b) {
        return find(a) == find(b);
    }
};


#endif