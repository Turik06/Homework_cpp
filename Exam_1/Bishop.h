#pragma once
// Bishop.h
#ifndef BISHOP_H
#define BISHOP_H

#include "ChessPiece.h"

class Bishop : public ChessPiece {
public:
    Bishop(Color color, Position position);

    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override;
    char getSymbol() const override;

    std::unique_ptr<ChessPiece> clone() const override;
};

#endif // BISHOP_H
