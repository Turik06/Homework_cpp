// Rook.cpp
#include "Rook.h"
#include "Board.h"
#include <cmath>

Rook::Rook(Color color, Position position)
    : ChessPiece(color, position) {}

std::vector<Move> Rook::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;

    // Ћадь€ может двигатьс€ только по горизонтали и вертикали
    const int dx[] = { 0, 1, 0, -1 };
    const int dy[] = { 1, 0, -1, 0 };

    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx[i] * step, position.y + dy[i] * step);
            if (!isValidPosition(newPos)) break;

            if (isPositionEmptyOrEnemy(board, newPos)) {
                moves.push_back(Move(position, newPos));

                // ≈сли на пути встретилась фигура противника, дальше идти нельз€
                if (board.getPieceAt(newPos) != nullptr) {
                    break;
                }
            }
            else {
                break;
            }
        }
    }

    return moves;
}

bool Rook::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) {
        return false;
    }

    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    // Ћадь€ может двигатьс€ только по горизонтали или вертикали
    if (dx != 0 && dy != 0) {
        return false;
    }

    // ѕровер€ем, нет ли фигур на пути
    int stepX = (dx != 0) ? dx / std::abs(dx) : 0;
    int stepY = (dy != 0) ? dy / std::abs(dy) : 0;

    Position checkPos = move.from;
    while (true) {
        checkPos.x += stepX;
        checkPos.y += stepY;

        if (checkPos == move.to) {
            break;
        }

        if (board.getPieceAt(checkPos) != nullptr) {
            return false; // Ќа пути есть фигура
        }
    }

    return isPositionEmptyOrEnemy(board, move.to);
}

PieceType Rook::getType() const {
    return PieceType::ROOK;
}

char Rook::getSymbol() const {
    return color == Color::WHITE ? 'R' : 'r';
}

std::unique_ptr<ChessPiece> Rook::clone() const {
    return std::make_unique<Rook>(color, position);
}
