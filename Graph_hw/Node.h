#pragma once
#include <string>
#include <set>

class Node {
    std::string name;
    std::set<Node*> neighbours;

    void addNeighbour(Node* neighbour);
    void removeNeighbour(Node* neighbour);

public:
    explicit Node(const std::string& name);
    ~Node() = default;

    const std::string& getName() const;

    using iterator = std::set<Node*>::const_iterator;
    iterator nb_begin() const;
    iterator nb_end() const;

    friend class Graph;
};
