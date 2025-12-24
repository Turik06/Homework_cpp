#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    int searchDepth = 3;
    bool logAlphaBeta = false;
    std::string logFilePath = "ai_log.txt";
    
    // Piece values
    int pawnValue = 100;
    int knightValue = 320;
    int bishopValue = 330;
    int rookValue = 500;
    int queenValue = 900;
    int kingValue = 20000;

    void load(const std::string& filename);
};

#endif // CONFIG_H
