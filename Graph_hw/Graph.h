#pragma once
#include "Node.h"
#include <set>
#include <string>

class Graph {
    std::set<Node*> nodes;

    Node* findNode(const std::string& name) const;

public:
    using iterator = std::set<Node*>::const_iterator;

    Graph() = default;
    explicit Graph(const std::string& file_name);
    ~Graph();

    void addNode(Node* node);
    void removeNode(Node* node);
    void addEdge(Node* begin, Node* end);
    void removeEdge(Node* begin, Node* end);

    iterator begin() const;
    iterator end() const;
};
