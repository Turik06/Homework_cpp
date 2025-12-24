#include "AntColonyOptimizer.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>
#include <iostream>

AntColonyOptimizer::AntColonyOptimizer(const Graph& graph, const std::vector<AntType>& antTypes, int numIterations, double evaporationRate)
    : graph(graph), numIterations(numIterations), evaporationRate(evaporationRate), rng(std::random_device{}()) {
    
    // Разворачиваем типы муравьев в единую колонию
    for (const auto& type : antTypes) {
    for (int i = 0; i < type.count; ++i) {
        colony.push_back({type.alpha, type.beta, type.name});
    }
}
    initializePheromones();
    logFile.open("aco_data.txt");
}

AntColonyOptimizer::~AntColonyOptimizer() {
    if (logFile.is_open()) logFile.close();
}

void AntColonyOptimizer::initializePheromones() {
    int n = graph.getNumVertices();
    pheromones.assign(n, std::vector<double>(n, 0.1));
}

// Передаем параметры (alpha, beta) внутрь
std::vector<int> AntColonyOptimizer::constructPath(int startVertexId, const AntParams& params) {
    int n = graph.getNumVertices();
    std::vector<int> path;
    std::vector<bool> visited(n, false);

    int currentVertex = startVertexId;
    path.push_back(currentVertex);
    visited[currentVertex] = true;

    for (int i = 0; i < n - 1; ++i) {
        int nextVertex = selectNextVertex(currentVertex, visited, params);
        if (nextVertex == -1) break;
        path.push_back(nextVertex);
        visited[nextVertex] = true;
        currentVertex = nextVertex;
    }
    return path;
}

// Используем params.alpha и params.beta вместо глобальных полей
int AntColonyOptimizer::selectNextVertex(int currentVertexId, const std::vector<bool>& visited, const AntParams& params) {
    int n = graph.getNumVertices();
    std::vector<double> probabilities(n, 0.0);
    double sum = 0.0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            double edgeWeight = graph.getEdgeWeight(currentVertexId, i);
            if (edgeWeight != std::numeric_limits<double>::infinity() && edgeWeight > 0) {
                // ВОТ ЗДЕСЬ ИСПОЛЬЗУЮТСЯ ИНДИВИДУАЛЬНЫЕ ПАРАМЕТРЫ
                double pheromone = std::pow(pheromones[currentVertexId][i], params.alpha);
                double visibility = std::pow(1.0 / edgeWeight, params.beta);
                probabilities[i] = pheromone * visibility;
                sum += probabilities[i];
            }
        }
    }

    if (sum == 0.0) return -1;

    std::uniform_real_distribution<double> dist(0.0, sum);
    double r = dist(rng);
    double currentSum = 0.0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && probabilities[i] > 0) {
            currentSum += probabilities[i];
            if (currentSum >= r) return i;
        }
    }
    return -1;
}

double AntColonyOptimizer::calculatePathLength(const std::vector<int>& path) {
    double length = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        length += graph.getEdgeWeight(path[i], path[i + 1]);
    }
    // Замыкаем путь (возврат в начало)
    if (!path.empty()) {
        length += graph.getEdgeWeight(path.back(), path[0]);
    }
    return length;
}

void AntColonyOptimizer::updatePheromones(const std::vector<std::vector<int>>& allPaths, const std::vector<double>& pathLengths) {
    int n = graph.getNumVertices();
    
    // Испарение
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pheromones[i][j] *= (1.0 - evaporationRate);
            if (pheromones[i][j] < 0.01) pheromones[i][j] = 0.01;
        }
    }

    // Добавление феромонов (все муравьи вносят вклад одинаково, независимо от типа)
    for (size_t k = 0; k < allPaths.size(); ++k) {
        double length = pathLengths[k];
        if (length > 0 && length != std::numeric_limits<double>::infinity()) {
            double contribution = 100.0 / length; 
            const auto& path = allPaths[k];
            for (size_t i = 0; i < path.size() - 1; ++i) {
                pheromones[path[i]][path[i+1]] += contribution;
                pheromones[path[i+1]][path[i]] += contribution;
            }
            // Замыкание
            pheromones[path.back()][path[0]] += contribution;
            pheromones[path[0]][path.back()] += contribution;
        }
    }
}

Result AntColonyOptimizer::run() {
    int n = graph.getNumVertices();
    Result bestResult;
    bestResult.length = std::numeric_limits<double>::infinity();
    int totalAnts = colony.size();

    std::cout << "Total ants in colony: " << totalAnts << std::endl;

    for (int iter = 0; iter < numIterations; ++iter) {
        std::vector<std::vector<int>> allPaths(totalAnts);
        std::vector<double> pathLengths(totalAnts);

        // Запускаем каждого муравья с его личными параметрами
        for (int i = 0; i < totalAnts; ++i) {
            allPaths[i] = constructPath(i % n, colony[i]); // Передаем colony[i] (параметры)
            pathLengths[i] = calculatePathLength(allPaths[i]);

            if (pathLengths[i] < bestResult.length) {
                bestResult.path = allPaths[i];
                bestResult.length = pathLengths[i];
                // Можно вывести, какой тип муравья нашел лучший путь
                // std::cout << "New best by " << colony[i].typeName << ": " << bestResult.length << std::endl;
            }
        }

        updatePheromones(allPaths, pathLengths);

        if (logFile.is_open()) {
            logFile << iter << " " << bestResult.length << "\n";
        }
    }
    return bestResult;
}