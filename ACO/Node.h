#ifndef NODE_H
#define NODE_H

#include <map>
#include <vector>

class Node {
public:
    Node(int id);

    void addEdge(Node* neighbor, double weight);
    double getWeight(Node* neighbor) const;
    int getId() const;
    const std::map<Node*, double>& getNeighbors() const;

private:
    int id;
    std::map<Node*, double> neighbors;
};

#endif // NODE_H
