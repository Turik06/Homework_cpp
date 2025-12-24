#include "Node.h"
#include <limits>

// Конструктор: инициализирует вершину с заданным идентификатором
Node::Node(int id) : id(id) {}

// Добавляет ребро от текущей вершины к соседней с указанным весом
void Node::addEdge(Node* neighbor, double weight) {
    neighbors[neighbor] = weight;
}

// Возвращает вес ребра к указанной соседней вершине
// Если ребро отсутствует, возвращает бесконечность
double Node::getWeight(Node* neighbor) const {
    auto it = neighbors.find(neighbor);
    if (it != neighbors.end()) {
        return it->second;
    }
    return std::numeric_limits<double>::infinity();
}

// Возвращает идентификатор текущей вершины
int Node::getId() const {
    return id;
}

// Возвращает список соседей текущей вершины и веса ребер к ним
const std::map<Node*, double>& Node::getNeighbors() const {
    return neighbors;
} 