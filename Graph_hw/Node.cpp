#include "Node.h"

Node::Node(const std::string& name) : name(name) {}

const std::string& Node::getName() const {
    return name;
}

Node::iterator Node::nb_begin() const {
    return neighbours.cbegin();
}

Node::iterator Node::nb_end() const {
    return neighbours.cend();
}

void Node::addNeighbour(Node* neighbour) {
    neighbours.insert(neighbour);
}

void Node::removeNeighbour(Node* neighbour) {
    neighbours.erase(neighbour);
}
