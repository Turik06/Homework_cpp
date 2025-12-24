#pragma once
// Rook.h
#ifndef ROOK_H
#define ROOK_H

#include "ChessPiece.h"

class Rook : public ChessPiece {
public:
    Rook(Color color, Position position);

    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override;
    char getSymbol() const override;

    std::unique_ptr<ChessPiece> clone() const override;
};

#endif // ROOK_H
