#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <vector>
#include <memory>
#include "ChessTypes.h"

class Board; // forward

class ChessPiece {
public:
    ChessPiece(Color c, Position p) : color(c), position(p) {}
    virtual ~ChessPiece() = default;

    Color getColor() const { return color; }
    Position getPosition() const { return position; }
    void setPosition(const Position& p) { position = p; }

    virtual std::vector<Move> getPossibleMoves(const Board& board) const = 0;
    virtual bool isValidMove(const Board& board, const Move& move) const = 0;
    virtual PieceType getType() const = 0;
    virtual char getSymbol() const = 0;
    virtual std::unique_ptr<ChessPiece> clone() const = 0;

protected:
    Color color;
    Position position;

    bool isValidPosition(const Position& p) const { return p.isValid(); }
    bool isPositionEmptyOrEnemy(const Board& board, const Position& p) const;
    bool isPositionEmpty(const Board& board, const Position& p) const;
};

#endif // CHESS_PIECE_H
