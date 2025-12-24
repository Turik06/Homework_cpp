#ifndef _LIAN_H
#define _LIAN_H

#include "map.h"
#include <queue>
#include <unordered_set>
#include <algorithm>

struct CompareF {
    bool operator()(const Cell* a, const Cell* b) const {
        return a->f > b->f;
    }
};

class LIAN {
private:
    Map& map;
    Cell* start = nullptr;
    Cell* end = nullptr;
    float stepLimit = 0.0f;
    float maxAngle = 0.0f;

    priority_queue<Cell*, std::vector<Cell*>, CompareF> openList;
    unordered_set<Cell*> openSet;
    unordered_set<Cell*> closedSet;

    float calculateH(Cell* current, Cell* end);
    vector<Cell*> getNeighbours(Cell* current);
    bool isLineOfSight(const Cell* from, const Cell* to);
public:
    LIAN(Map& map, Cell* start, Cell* end, float stepLimit = 0.0f, float maxTurnAngle = 180.0f);
    vector<Cell*> findPath();
};

#endif //_LIAN_H
