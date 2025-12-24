#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include "ChessTypes.h"

class ChessPiece; // Предварительное объявление

class Board {
private:
    std::unique_ptr<ChessPiece> board[8][8];

public:
    Board();
    ~Board();
    Board(const Board& other);
    Board& operator=(const Board& other);

    void clear();
    void addPiece(std::unique_ptr<ChessPiece> piece);
    void removePiece(const Position& position);
    ChessPiece* getPieceAt(const Position& position) const;

    bool makeMove(const Move& move);

    std::vector<ChessPiece*> getPiecesByColor(Color color) const;
    Position findKing(Color color) const;

    bool isPositionAttacked(const Position& position, Color attackingColor) const;
    bool isKingInCheck(Color color) const;
    bool isKingInCheckmate(Color color) const;

    void print() const;
};

#endif // BOARD_H