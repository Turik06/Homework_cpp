#include "BFS.h"

BFS::BFS(const Graph& graph) : graph(graph) {}

bool BFS::connected(Node* start, Node* target) {
    if (!start || !target) return false;

    std::queue<Node*> q;
    std::set<Node*> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        Node* current = q.front(); q.pop();
        for (auto it = current->nb_begin(); it != current->nb_end(); ++it) {
            if (visited.find(it->first) == visited.end()) {
                visited.insert(it->first);
                q.push(it->first);
            }
        }
    }

    return false;
}

std::set<Node*> BFS::getComponent(Node* start) {
    std::set<Node*> component;
    if (!start) return component;

    std::queue<Node*> q;
    std::set<Node*> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        Node* current = q.front(); q.pop();
        component.insert(current);

        for (auto it = current->nb_begin(); it != current->nb_end(); ++it) {
            if (visited.find(it->first) == visited.end()) {
                visited.insert(it->first);
                q.push(it->first);
            }
        }
    }

    return component;
}

