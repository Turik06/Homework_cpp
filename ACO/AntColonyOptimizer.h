#ifndef ANT_COLONY_OPTIMIZER_H
#define ANT_COLONY_OPTIMIZER_H

#include "Graph.h"
#include "Config.h" // Подключаем, чтобы видеть структуру AntType
#include <vector>
#include <random>
#include <fstream>

struct Result {
    std::vector<int> path;
    double length;
};

// Структура параметров конкретного муравья
struct AntParams {
    double alpha;
    double beta;
    std::string typeName;
};

class AntColonyOptimizer {
public:
    // Конструктор теперь принимает вектор типов муравьев
    AntColonyOptimizer(const Graph& graph, const std::vector<AntType>& antTypes, int numIterations, double evaporationRate);
    ~AntColonyOptimizer();

    Result run();

private:
    const Graph& graph;
    int numIterations;
    double evaporationRate;
    
    // Список параметров для каждого муравья в колонии (развернутый список)
    std::vector<AntParams> colony; 

    std::vector<std::vector<double>> pheromones;
    std::mt19937 rng;
    std::ofstream logFile;

    void initializePheromones();
    
    // Теперь constructPath принимает параметры конкретного муравья
    std::vector<int> constructPath(int startVertex, const AntParams& params);
    
    double calculatePathLength(const std::vector<int>& path);
    void updatePheromones(const std::vector<std::vector<int>>& allPaths, const std::vector<double>& pathLengths);
    
    // selectNextVertex тоже должен знать alpha и beta текущего муравья
    int selectNextVertex(int currentVertex, const std::vector<bool>& visited, const AntParams& params);
};

#endif // ANT_COLONY_OPTIMIZER_H