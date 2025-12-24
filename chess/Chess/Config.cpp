#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Config::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open config file: " << filename << ". Using defaults." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                // Remove potential carriage return
                if (!value.empty() && value.back() == '\r') value.pop_back();
                
                if (key == "searchDepth") searchDepth = std::stoi(value);
                else if (key == "logAlphaBeta") logAlphaBeta = (value == "true" || value == "1");
                else if (key == "logFilePath") logFilePath = value;
                else if (key == "pawnValue") pawnValue = std::stoi(value);
                else if (key == "knightValue") knightValue = std::stoi(value);
                else if (key == "bishopValue") bishopValue = std::stoi(value);
                else if (key == "rookValue") rookValue = std::stoi(value);
                else if (key == "queenValue") queenValue = std::stoi(value);
            }
        }
    }
    std::cout << "Config loaded. Depth: " << searchDepth << ", Log: " << logAlphaBeta << std::endl;
}
