#ifndef KING_H
#define KING_H

#include "ChessPiece.h"
#include <memory>

class King : public ChessPiece {
public:
    King(Color c, Position p) : ChessPiece(c, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::KING; }
    char getSymbol() const override { return getColor() == Color::WHITE ? 'K' : 'k'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<King>(*this);
    }
};

#endif // KING_H
