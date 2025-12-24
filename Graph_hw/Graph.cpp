#include "Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Деструктор: освобождает память для всех узлов
Graph::~Graph() {
    for (auto node : nodes) {
        delete node;
    }
}

// Конструктор: загружает граф из файла
Graph::Graph(const std::string& file_name, bool is_weighted) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Пропускаем первую строку

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string src_name, dst_name;
        int weight = 1;

        if (!(iss >> src_name >> dst_name)) continue;
        if (is_weighted && !(iss >> weight)) weight = 1;

        Node* srcNode = findNode(src_name);
        if (!srcNode) {
            srcNode = new Node(src_name);
            addNode(srcNode);
        }

        Node* dstNode = findNode(dst_name);
        if (!dstNode) {
            dstNode = new Node(dst_name);
            addNode(dstNode);
        }
        addEdge(srcNode, dstNode, weight);
    }
}

// Поиск узла по имени
Node* Graph::findNode(const std::string& name) const {
    for (Node* node : nodes) {
        if (node->getName() == name) {
            return node;
        }
    }
    return nullptr;
}

// Добавление узла в граф
void Graph::addNode(Node* node) {
    nodes.insert(node);
}

// Добавление ребра между двумя узлами
void Graph::addEdge(Node* begin, Node* end, int weight) {
    if (!begin || !end) return;
    end->addNeighbour(begin, weight);
    begin->addNeighbour(end, weight);
}

// Итераторы для обхода узлов графа
Graph::iterator Graph::begin() const {
    return nodes.begin();
}

Graph::iterator Graph::end() const {
    return nodes.end();
}   

void Graph::removeEdge(Node* begin, Node* end) {
    if (!begin || !end) return; // Проверяем, что узлы существуют

    // Удаляем связь между узлами
    begin->removeNeighbour(end);
    end->removeNeighbour(begin);
}