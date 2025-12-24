#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include "Board.h"
#include "ChessTypes.h"
#include "MateAnalyzer.h"

class Game {
public:
    Game();
    bool loadFromFile(const std::string& filename);
    bool canWhiteCheckmateInOneMove() const;
    bool canBlackCheckmateInOneMove() const;
    bool canWhiteCheckmateInTwoMoves() const;
    bool canBlackCheckmateInTwoMoves() const;
    bool canWhiteCheckmateInNMoves(int n) const;
    bool canBlackCheckmateInNMoves(int n) const;
    std::vector<Move> getWhiteMatingSequence(int n) const;
    std::vector<Move> getBlackMatingSequence(int n) const;
    void printBoard() const;

private:
    Board board;
    MateAnalyzer mateAnalyzer;
    std::unique_ptr<ChessPiece> createPiece(char symbol, const Position& pos);
};

#endif // GAME_H
