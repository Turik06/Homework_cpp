#include "ChessPiece.h"
#include "Board.h"

// Проверяет, пуста ли позиция или занята фигурой противника
bool ChessPiece::isPositionEmptyOrEnemy(const Board& board, const Position& p) const {
    if (!p.isValid()) return false; // Невалидная позиция — сразу false
    ChessPiece* piece = board.getPieceAt(p);
    return (piece == nullptr) || (piece->getColor() != this->getColor());
}

// Проверяет, пуста ли позиция
bool ChessPiece::isPositionEmpty(const Board& board, const Position& p) const {
    if (!p.isValid()) return false; // Невалидная позиция — false
    return (board.getPieceAt(p) == nullptr);
}
