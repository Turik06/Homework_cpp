#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <set>
#include <map>
#include <limits>
#include "Board.h"
#include "ChessTypes.h"

class ChessPiece;

class Game {
public:
    Game();

    bool loadFromFile(const std::string& filename);
    void printBoard() const;
    bool isCurrentPositionMate() const;

    std::map<int, std::set<std::string>> findAllMateSequences(int maxDepthFullMoves = 3) const;

private:
    Board board;

    std::unique_ptr<ChessPiece> createPiece(char symbol, const Position& pos) const;
    std::vector<Move> generateOrderedMoves(const Board& currentBoard, Color color) const;
    int evaluateMove(const Board& currentBoard, const Move& move, Color color) const;
    int getPieceValue(PieceType type) const;
    std::vector<Move> getAllLegalMoves(const Board& currentBoard, Color color) const;
    std::string movesToString(const std::vector<Move>& moves) const;

    int evaluateBoard(const Board& currentBoard, Color playerToMove) const;
    int alphaBetaSearch(Board currentBoard, int depthInPlies, int alpha, int beta, Color playerToMove,
        std::vector<Move>& bestMoveSequenceForThisNode) const;

    std::set<std::string> findForcedMate_AlphaBeta(int numFullMoves) const;

    
    static const int MATE_SCORE = 1000000;
    static const int INFINITY_SCORE = MATE_SCORE + 1000;
};

#endif // GAME_H
