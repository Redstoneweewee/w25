#include "WordList.h"
#include "Heap.h"

WordList::WordList(std::istream& stream) {

}

Heap WordList::correct(const std::vector<Point>& points, size_t maxcount, float cutoff) const {
    return Heap(maxcount);
}