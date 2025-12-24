#ifndef _MAP_H
#define _MAP_H

#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

struct Cell {
    int x = 0;
    int y = 0;
    bool obstacle = false;
    
    Cell() = default;
    Cell(int x_, int y_, bool obs) : x(x_), y(y_), obstacle(obs) {}

    float g = 0.0f;
    float h = 0.0f;
    float f = 0.0f;
    Cell* parent = nullptr;
    float angle = 0.0f;

    float distanceTo(const Cell& other) const;
    bool operator==(const Cell& other) const;
};

class Map {
private:
    vector<vector<Cell>> grid;
public:
    int width = 0;
    int height = 0;

    Map(const std::string& filename);
    Cell* getCell(int x, int y);
    bool isFree(int x, int y) const;
    const vector<vector<Cell>>& getGrid() const;
};

#endif //_MAP_H

