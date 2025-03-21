#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <unordered_map>
#include <unordered_set>

template <typename T>
class DisjointSet {
private:
    std::unordered_map<T, T> parent; // Maps element -> parent
    std::unordered_map<T, size_t> rank;   // Maps element -> rank
    
public:
    std::unordered_set<T> allParents;

    // Add a new element as its own set
    void add(T element) {
        if(parent.find(element) == parent.end()) {
            parent[element] = element;
            allParents.insert(element);
            rank[element] = 1;
        }
    }

    /**
     * Find with path compression
     * If not in the set, returns NULL
     */
    T find(T element) {
        if (parent.find(element) == parent.end()) {
            return NULL;
        }
        if (parent[element] != element) {
            parent[element] = find(parent[element]);
        }
        return parent[element];
    }

    /**
     * Only used for tempRegions!!!!!!!!
     */
    void erase(T element) {
        parent.erase(element);
    }

    // Unites two sets
    void unite(T a, T b) {
        T rootA = find(a);
        T rootB = find(b);

        if (rootA != rootB) {
            // Union by rank
            if (rank[rootA] > rank[rootB]) {
                parent[rootB] = rootA;
                allParents.insert(rootA);
                allParents.erase(rootB);
            } 
            else if (rank[rootA] < rank[rootB]) {
                parent[rootA] = rootB;
                allParents.insert(rootB);
                allParents.erase(rootA);
            } 
            else {
                parent[rootB] = rootA;
                allParents.insert(rootA);
                allParents.erase(rootB);
                rank[rootA]++;
            }
        }
    }

    bool connected(T a, T b) {
        return find(a) == find(b);
    }

    bool isParent(T element) {
        return allParents.find(element) == allParents.end();
    }
};


#endif