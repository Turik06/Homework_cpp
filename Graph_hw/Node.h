#pragma once
#include <string>
#include <map>

class Node {
    std::string name;
    std::map<Node*, int> neighbours; 

    void addNeighbour(Node* neighbour, int weight);
    void removeNeighbour(Node* neighbour);

public:
    explicit Node(const std::string& name);
    const std::string& getName() const;

    using iterator = std::map<Node*, int>::const_iterator;
    iterator nb_begin() const;
    iterator nb_end() const;

    friend class Graph;
};