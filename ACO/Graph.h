#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include <vector>
#include <map>

class Graph {
public:
    Graph();
    ~Graph();

    void addNode(int id);
    void addEdge(int fromId, int toId, double weight);
    Node* getNode(int id) const;
    int getNumVertices() const;
    const std::vector<Node*>& getNodes() const;
    double getEdgeWeight(int fromId, int toId) const;

    void printGraph() const;

private:
    std::vector<Node*> nodes;
    std::map<int, Node*> nodeMap;
};

#endif // GRAPH_H 