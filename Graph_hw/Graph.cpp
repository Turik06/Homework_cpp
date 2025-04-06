#include "Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>

Graph::~Graph() {
    for (auto node : nodes) {
        delete node;
    }
}

Node* Graph::findNode(const std::string& name) const {
    for (Node* node : nodes) {
        if (node->getName() == name) {
            return node;
        }
    }
    return nullptr;
}

void Graph::addNode(Node* node) {
    nodes.insert(node);
}

void Graph::removeNode(Node* node) {
    nodes.erase(node);
    for (auto n : nodes) {
        n->removeNeighbour(node);
    }
}

void Graph::addEdge(Node* begin, Node* end) {
    if (!begin || !end) return;
    if (nodes.find(begin) == nodes.end()) return;
    if (nodes.find(end) == nodes.end()) return;

    begin->addNeighbour(end);
    end->addNeighbour(begin);
}

void Graph::removeEdge(Node* begin, Node* end) {
    if (begin) begin->removeNeighbour(end);
    if (end) end->removeNeighbour(begin);
}

Graph::iterator Graph::begin() const {
    return nodes.begin();
}

Graph::iterator Graph::end() const {
    return nodes.end();
}

Graph::Graph(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Пропустить заголовок

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string src, dst;
        if (!(iss >> src >> dst)) continue;

        Node* srcNode = findNode(src);
        if (!srcNode) {
            srcNode = new Node(src);
            addNode(srcNode);
        }

        Node* dstNode = findNode(dst);
        if (!dstNode) {
            dstNode = new Node(dst);
            addNode(dstNode);
        }

        addEdge(srcNode, dstNode);
    }
}
