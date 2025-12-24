#include "Dijkstra.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>

Dijkstra::Dijkstra(const Graph& graph) : graph(graph) {}

// Находит кратчайший путь от 'start' до 'end' с помощью алгоритма Дейкстры.
std::pair<int, std::vector<Node*>> Dijkstra::findShortestPath(Node* start, Node* end) {
    std::map<Node*, int> distances;
    std::map<Node*, Node*> predecessors;
    // Очередь с приоритетом, хранит узлы с наименьшим расстоянием наверху.
    std::priority_queue<std::pair<int, Node*>, std::vector<std::pair<int, Node*>>, std::greater<std::pair<int, Node*>>> pq;

    // Шаг 1: Инициализация. Все расстояния - бесконечность, кроме стартового.
    for (auto node_it = graph.begin(); node_it != graph.end(); ++node_it) {
        distances[*node_it] = std::numeric_limits<int>::max();
        predecessors[*node_it] = nullptr;
    }
    distances[start] = 0;
    pq.push({0, start});

    // Шаг 2: Основной цикл. Работаем, пока есть узлы для обработки.
    while (!pq.empty()) {
        Node* u = pq.top().second;
        pq.pop();

        // Если извлекли конечный узел, путь найден.
        if (u == end) {
            break;
        }

        // Шаг 3: "Релаксация". Проверяем всех соседей и улучшаем пути до них.
        for (auto it = u->nb_begin(); it != u->nb_end(); ++it) {
            Node* v = it->first;
            int weight = it->second;

            // Если путь через 'u' до 'v' короче, обновляем информацию.
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                predecessors[v] = u;
                pq.push({distances[v], v});
            }
        }
    }

    // Шаг 4: Восстановление пути. Двигаемся от 'end' к 'start' по "родителям".
    std::vector<Node*> path;
    if (distances[end] != std::numeric_limits<int>::max()) {
        Node* current = end;
        while (current != nullptr) {
            path.push_back(current);
            current = predecessors[current];
        }
        std::reverse(path.begin(), path.end());
    }

    return {distances.count(end) ? distances[end] : std::numeric_limits<int>::max(), path};
}