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
