#include "ChessPiece.h"
#include "Board.h"

// ���������, ����� �� ������� ��� ������ ������� ����������
bool ChessPiece::isPositionEmptyOrEnemy(const Board& board, const Position& p) const {
    if (!p.isValid()) return false; // ���������� ������� � ����� false
    ChessPiece* piece = board.getPieceAt(p);
    return (piece == nullptr) || (piece->getColor() != this->getColor());
}

// ���������, ����� �� �������
bool ChessPiece::isPositionEmpty(const Board& board, const Position& p) const {
    if (!p.isValid()) return false; // ���������� ������� � false
    return (board.getPieceAt(p) == nullptr);
}
