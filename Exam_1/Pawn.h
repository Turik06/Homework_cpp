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
    // ���������, ����� �� ����� ��������� �� ��� ������ �����
    bool canMoveForwardTwo(const Board& board) const;

    // ���������, ����� �� ����� ����� ������ �� ���������
    bool canCaptureDiagonally(const Board& board, const Position& target) const;
};

#endif // PAWN_H
