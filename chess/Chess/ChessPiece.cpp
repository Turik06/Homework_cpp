#include "ChessPiece.h"
#include "Board.h"

// Проверяет, является ли позиция пустой или занятой фигурой противника
bool ChessPiece::isPositionEmptyOrEnemy(const Board& board, const Position& p) const {
    if (!p.isValid()) return false;
    ChessPiece* piece = board.getPieceAt(p);
    return (piece == nullptr) || (piece->getColor() != this->getColor());
}

// Проверяет, является ли позиция пустой
bool ChessPiece::isPositionEmpty(const Board& board, const Position& p) const {
    if (!p.isValid()) return false;
    return (board.getPieceAt(p) == nullptr);
}