// Knight.cpp
#include "Knight.h"
#include "Board.h"
#include <cmath>

Knight::Knight(Color color, Position position)
    : ChessPiece(color, position) {}

std::vector<Move> Knight::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;

    // Конь ходит буквой "Г" - на 2 клетки по одной оси и на 1 по другой
    const int dx[] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    const int dy[] = { 2, 1, -1, -2, -2, -1, 1, 2 };

    for (int i = 0; i < 8; i++) {
        Position newPos(position.x + dx[i], position.y + dy[i]);
        if (isValidPosition(newPos) && isPositionEmptyOrEnemy(board, newPos)) {
            moves.push_back(Move(position, newPos));
        }
    }

    return moves;
}

bool Knight::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) {
        return false;
    }

    int dx = std::abs(move.to.x - move.from.x);
    int dy = std::abs(move.to.y - move.from.y);

    // Конь ходит буквой "Г" - на 2 клетки по одной оси и на 1 по другой
    if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) {
        return false;
    }

    return isPositionEmptyOrEnemy(board, move.to);
}

PieceType Knight::getType() const {
    return PieceType::KNIGHT;
}

char Knight::getSymbol() const {
    return color == Color::WHITE ? 'N' : 'n';
}

std::unique_ptr<ChessPiece> Knight::clone() const {
    return std::make_unique<Knight>(color, position);
}
