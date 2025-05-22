#pragma once
// Queen.h
#ifndef QUEEN_H
#define QUEEN_H

#include "ChessPiece.h"

class Queen : public ChessPiece {
public:
    Queen(Color color, Position position);

    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override;
    char getSymbol() const override;

    // Клонирование для создания копии доски
    std::unique_ptr<ChessPiece> clone() const override;
};

#endif // QUEEN_H
