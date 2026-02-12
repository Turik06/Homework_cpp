#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <map>
#include "Board.h"
#include "ChessTypes.h"

class ChessPiece;

class Game {
public:
    Game();
    void setupNewGame();
    bool makeMove(const Move& move);
    Move findBestMove(int depth);
    void printBoard() const;
    Color getCurrentTurn() const { return turn; }
    bool isGameOver(std::string& reason);

private:
    Board board;
    Color turn;

    static const int MATE_SCORE = 1000000;
    static const int INFINITY_SCORE = 1000001;

    std::unique_ptr<ChessPiece> createPiece(char sym, const Position& pos) const;
    std::vector<Move> getAllLegalMoves(const Board& currentBoard, Color color) const;
    std::vector<Move> generateOrderedMoves(const Board& currentBoard, Color color) const;
    
    int getPieceValue(PieceType type) const;
    
    int evaluateBoard(const Board& currentBoard) const;
    int alphaBeta(Board currentBoard, int depth, int alpha, int beta, bool maximizingPlayer) const;
};

#endif // GAME_H