<<<<<<< HEAD
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <limits>
#include <algorithm>
#include <iomanip>

#include "Graph.h"
#include "Dijkstra.h"
#include "BFS.h"

// Вспомогательная функция для сортировки компонентов по убыванию размера.
bool compareComponents(const std::set<Node*>& a, const std::set<Node*>& b) {
    return a.size() > b.size();
}

// Вспомогательная функция для поиска индекса компонента, которому принадлежит узел.
int find_component_index(const std::vector<std::set<Node*>>& components, Node* node) {
    for (size_t i = 0; i < components.size(); ++i) {
        if (components[i].find(node) != components[i].end()) {
            return static_cast<int>(i + 1);
        }
    }
    return -1;
}

int main() {
    // Шаг 1: Загрузка графа из файла.
    Graph graph("1000.txt", true);
    std::cout << "Graph loaded successfully from '1000.txt'.\n\n";

    // Шаг 2: Поиск всех компонент связности в графе с помощью BFS (поиска в ширину).
    // Это позволяет работать с несвязными графами, анализируя каждый "островок" узлов отдельно.
    BFS bfs(graph);
    std::set<Node*> visited_nodes;
    std::vector<std::set<Node*>> components;

    for (auto it = graph.begin(); it != graph.end(); ++it) {
        Node* node = *it;
        if (visited_nodes.find(node) == visited_nodes.end()) {
            std::set<Node*> new_component = bfs.getComponent(node);
            components.push_back(new_component);
            visited_nodes.insert(new_component.begin(), new_component.end());
        }
    }

    // Сортируем компоненты по размеру и выводим информацию о них.
    std::sort(components.begin(), components.end(), compareComponents);
    std::cout << "Found " << components.size() << " connected components in the graph.\n";
    std::cout << "-------------------------------------------\n";
    for (size_t i = 0; i < components.size(); ++i) {
        std::cout << "Component " << std::setw(3) << i + 1 << ": " << components[i].size() << " nodes\n";
    }
    std::cout << "-------------------------------------------\n\n";

    if (components.empty()) {
        std::cout << "The graph is empty. Exiting.\n";
        return 0;
    }

    // Шаг 3: Пользователь выбирает, с какой компонентой связности он хочет работать.
    int choice = -1;
    while (choice < 1 || choice > static_cast<int>(components.size())) {
        std::cout << "Please select a component to analyze (enter a number from 1 to " << components.size() << "): ";
        std::cin >> choice;
        // Проверка корректности ввода.
        if (std::cin.fail() || choice < 1 || choice > static_cast<int>(components.size())) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input. Please try again.\n";
            choice = -1;
        }
    }
    const std::set<Node*>& selected_component = components[choice - 1];
    std::cout << "\nYou have selected component " << choice << " with " << selected_component.size() << " nodes.\n\n";

    // Шаг 4: Пользователь вводит имена начального и конечного узлов.
    // Программа проверяет, что узлы существуют и принадлежат выбранной компоненте.
    std::string startNodeName, endNodeName;
    Node* startNode = nullptr;
    Node* endNode = nullptr;
    
    while (true) {
        std::cout << "Enter start node name: ";
        std::cin >> startNodeName;
        startNode = graph.findNode(startNodeName);

        if (!startNode) {
            std::cerr << "Error: Node '" << startNodeName << "' does not exist in the graph. Please try again.\n";
            continue;
        }
        if (selected_component.find(startNode) == selected_component.end()) {
            std::cerr << "Error: Node '" << startNodeName << "' is not in the selected component " << choice << ".\n";
            continue;
        }
        break;
    }
    
    while (true) {
        std::cout << "Enter end node name: ";
        std::cin >> endNodeName;
        endNode = graph.findNode(endNodeName);

        if (!endNode) {
            std::cerr << "Error: Node '" << endNodeName << "' does not exist in the graph. Please try again.\n";
            continue;
        }
        if (selected_component.find(endNode) == selected_component.end()) {
            std::cerr << "Error: Node '" << endNodeName << "' is not in the selected component " << choice << ".\n";
            continue;
        }
        if (startNode == endNode) {
            std::cerr << "Error: Start and end nodes are the same. Please enter a different end node.\n";
            continue;
        }
        break;
    }

    // Шаг 5: Запуск алгоритма Дейкстры для поиска кратчайшего пути.
    Dijkstra dijkstra(graph);
    auto result = dijkstra.findShortestPath(startNode, endNode);
    int distance = result.first;
    std::vector<Node*> path = result.second;

    // Шаг 6: Вывод результата - длины и самого пути.
    if (distance == std::numeric_limits<int>::max() || path.empty()) {
        std::cout << "\nNo path found between " << startNodeName << " and " << endNodeName << "." << std::endl;
    } else {
        std::cout << "\nShortest path distance: " << distance << std::endl;
        std::cout << "Path: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i]->getName() << (i == path.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
    }

    std::cout << "\nDone.\n";
    return 0;
}
=======
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include "BFS.h" 

void exportComponents(const Graph& graph, const std::string& prefix) {
    std::set<Node*> unvisited(graph.begin(), graph.end());
    BFS bfs(graph);
    int componentIndex = 0;

    while (!unvisited.empty()) {
        Node* start = *unvisited.begin();
        std::set<Node*> component = bfs.getComponent(start);

        std::string fileName = prefix + "_component_" + std::to_string(componentIndex++) + ".txt";
        std::ofstream out(fileName);
        out << "Source\tTarget\n";

        for (Node* node : component) {
            for (auto it = node->nb_begin(); it != node->nb_end(); ++it) {
                if (component.count(*it) && node->getName() < (*it)->getName()) {
                    out << node->getName() << "\t" << (*it)->getName() << "\n";
                }
            }
        }

        for (Node* n : component) {
            unvisited.erase(n);
        }
    }
}


int main() {
    Graph testGraph("TestGraph.txt");
    std::cout << "Processing TestGraph.txt...\n";
    exportComponents(testGraph, "TestGraph");

    Graph bigGraph("1000.csv");
    std::cout << "Processing 1000.csv...\n";
    exportComponents(bigGraph, "Graph1000");

    std::cout << "Done.\n";
    return 0;
}
>>>>>>> dc8fac3bcc0ddc407441edcd76c8c40939dcab99
