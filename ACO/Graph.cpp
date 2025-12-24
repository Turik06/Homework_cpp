#include "Graph.h"
#include <iostream>
#include <iomanip>
#include <limits>

// Конструктор графа
Graph::Graph() {}

// Деструктор графа: освобождает память, выделенную для узлов
Graph::~Graph() {
    for (Node* node : nodes) {
        delete node;
    }
}

// Добавляет вершину в граф, если она еще не существует
void Graph::addNode(int id) {
    if (nodeMap.find(id) == nodeMap.end()) {
        Node* newNode = new Node(id);
        nodes.push_back(newNode);
        nodeMap[id] = newNode;
    }
}

// Добавляет ребро между двумя вершинами с указанным весом (для ориентированного графа)
void Graph::addEdge(int fromId, int toId, double weight) {
    Node* fromNode = getNode(fromId);
    Node* toNode = getNode(toId);
    if (fromNode && toNode) {
        fromNode->addEdge(toNode, weight);
        // Убрали добавление обратного ребра - граф ориентированный
    }
}

// Возвращает указатель на вершину по ее идентификатору
Node* Graph::getNode(int id) const {
    auto it = nodeMap.find(id);
    if (it != nodeMap.end()) {
        return it->second;
    }
    return nullptr;
}

// Возвращает количество вершин в графе
int Graph::getNumVertices() const {
    return nodes.size();
}

// Возвращает список всех вершин графа
const std::vector<Node*>& Graph::getNodes() const {
    return nodes;
}

// Возвращает вес ребра между двумя вершинами, если оно существует
double Graph::getEdgeWeight(int fromId, int toId) const {
    Node* fromNode = getNode(fromId);
    Node* toNode = getNode(toId);
    if (fromNode && toNode) {
        return fromNode->getWeight(toNode);
    }
    return std::numeric_limits<double>::infinity();
}

// Выводит граф в виде списка смежности
void Graph::printGraph() const {
    std::cout << "Graph (Adjacency List):" << std::endl;
    for (Node* node : nodes) {
        std::cout << "Node " << node->getId() << ": ";
        for (const auto& pair : node->getNeighbors()) {
            std::cout << "(" << pair.first->getId() << ", " << pair.second << ") ";
        }
        std::cout << std::endl;
    }
}