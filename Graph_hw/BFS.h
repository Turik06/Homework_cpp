#pragma once
#include "Graph.h"
#include <queue>
#include <set>

class BFS {
    const Graph& graph;

public:
    explicit BFS(const Graph& graph);
    bool connected(Node* start, Node* target);
    std::set<Node*> getComponent(Node* start);
};
