#pragma once
#include "Graph.h"
#include <vector>
#include <map>

class Dijkstra {
    const Graph& graph;

public:
    explicit Dijkstra(const Graph& graph);
    std::pair<int, std::vector<Node*>> findShortestPath(Node* start, Node* end);
};