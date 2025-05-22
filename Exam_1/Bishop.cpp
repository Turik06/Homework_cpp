#include "Bishop.h"
#include "Board.h"
#include <cmath>

Bishop::Bishop(Color color, Position position)
    : ChessPiece(color, position) {}

std::vector<Move> Bishop::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;

    const int dx[] = { 1, 1, -1, -1 };
    const int dy[] = { 1, -1, -1, 1 };

    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx[i] * step, position.y + dy[i] * step);
            if (!isValidPosition(newPos)) break;

            if (isPositionEmptyOrEnemy(board, newPos)) {
                moves.push_back(Move(position, newPos));
                if (board.getPieceAt(newPos) != nullptr) break;
            }
            else break;
        }
    }

    return moves;
}

bool Bishop::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position || move.from == move.to) {
        return false;
    }

    const int dx = move.to.x - move.from.x;
    const int dy = move.to.y - move.from.y;


    const bool isDiagonal = (std::abs(dx) == std::abs(dy)) ? true : false;
    if (!isDiagonal) return false;

 
    const int stepX = (dx != 0) ? ((dx > 0) ? 1 : -1) : 0;
    const int stepY = (dy != 0) ? ((dy > 0) ? 1 : -1) : 0;

    Position checkPos = move.from;
    while (checkPos != move.to) {
        checkPos.x += stepX;
        checkPos.y += stepY;

        if (checkPos == move.to) break;
        if (board.getPieceAt(checkPos) != nullptr) return false;
    }

    return isPositionEmptyOrEnemy(board, move.to);
}

PieceType Bishop::getType() const {
    return PieceType::BISHOP;
}

char Bishop::getSymbol() const {
    return color == Color::WHITE ? 'B' : 'b';
}

std::unique_ptr<ChessPiece> Bishop::clone() const {
    return std::make_unique<Bishop>(color, position);
}
