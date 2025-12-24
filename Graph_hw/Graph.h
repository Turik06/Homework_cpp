#pragma once
#include "Node.h"
#include <set>
#include <string>

class Graph {
    std::set<Node*> nodes;

public:
    Graph() = default;
    explicit Graph(const std::string& file_name, bool is_weighted = false);
    ~Graph();

    Node* findNode(const std::string& name) const;
    void addNode(Node* node);
    void addEdge(Node* begin, Node* end, int weight = 1);

    using iterator = std::set<Node*>::const_iterator;
    iterator begin() const;
    iterator end() const;
    void removeEdge(Node* begin, Node* end);
};