#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Config.h"
#include <string>
#include <vector>
#include <fstream>

const int INFINITY_SCORE = 1000000;
const int MATE_SCORE = 900000;

class Game {
public:
    Game();
    void setupNewGame();
    void loadConfig(const std::string& filename);
    bool makeMove(const Move& move);
    void printBoard() const;
    bool isGameOver(std::string& reason);
    Move findBestMove(int depth = -1); // Default -1 to use config
    Color getCurrentTurn() const { return turn; }
    std::vector<Move> getAllLegalMoves(const Board& currentBoard, Color color) const;

private:
    Board board;
    Color turn;
    Config config;

    int getPieceValue(PieceType type) const;
    int evaluateBoard(const Board& currentBoard) const;
    int alphaBeta(Board currentBoard, int depth, int alpha, int beta, bool maximizingPlayer, std::ofstream* logger, std::string branchId);
};

#endif // GAME_H