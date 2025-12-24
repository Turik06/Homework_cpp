#pragma once
// Pawn.h
#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"

class Pawn : public ChessPiece {
public:
    Pawn(Color color, Position position);

    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override;
    char getSymbol() const override;

    std::unique_ptr<ChessPiece> clone() const override;

private:
    // Проверяет, может ли пешка двигаться на две клетки вперёд
    bool canMoveForwardTwo(const Board& board) const;

    // Проверяет, может ли пешка взять фигуру по диагонали
    bool canCaptureDiagonally(const Board& board, const Position& target) const;
};

#endif // PAWN_H
