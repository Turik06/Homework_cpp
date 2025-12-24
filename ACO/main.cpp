#include <iostream>
#include <fstream>
#include <locale>
#include <windows.h> 
#include "Graph.h"
#include "AntColonyOptimizer.h"
#include "Config.h" // Подключаем наш новый конфиг

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    setlocale(LC_ALL, "en_US.UTF-8");

    // 1. Загрузка конфигурации
    Config config;
    if (!config.loadFromFile("config.txt")) {
        std::cerr << "Не удалось загрузить config.txt, используются настройки по умолчанию." << std::endl;
    }
    config.print(); // Выводим настройки для проверки

    // 2. Открытие файла графа из конфига
    std::ifstream inputFile(config.input_file);
    if (!inputFile) {
        std::cerr << "Ошибка: не удалось открыть файл графа: " << config.input_file << std::endl;
        return 1;
    }

    int numVertices;
    inputFile >> numVertices;
    Graph graph;

    for (int i = 0; i < numVertices; ++i) {
        graph.addNode(i);
    }

    int from, to;
    double weight;
    while (inputFile >> from >> to >> weight) {
        graph.addEdge(from, to, weight);
    }
    inputFile.close();

    std::cout << "Граф загружен. Запуск алгоритма..." << std::endl;
    
    // 3. Передаем параметры из конфига в оптимизатор
    AntColonyOptimizer optimizer(graph, 
                                  config.ant_types,      // Вектор типов муравьев
                                  config.num_iterations, // Кол-во итераций
                                  config.evaporation_rate); // Скорость испарения

    Result result = optimizer.run();

    std::cout << "Лучшая длина пути: " << result.length << std::endl;
    
    // Вывод пути буквами (если граф маленький) или числами
    std::cout << "Путь: ";
    for (size_t i = 0; i < result.path.size(); ++i) {
        std::cout << result.path[i] << (i == result.path.size() - 1 ? "" : " -> ");
    }
    std::cout << std::endl;

    return 0;
}