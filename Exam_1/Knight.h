#pragma once
// Knight.h
#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"

class Knight : public ChessPiece {
public:
    Knight(Color color, Position position);

    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override;
    char getSymbol() const override;

    std::unique_ptr<ChessPiece> clone() const override;
};

#endif // KNIGHT_H
