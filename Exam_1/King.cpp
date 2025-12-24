#include "King.h"
#include "Board.h"
#include <cmath>

std::vector<Move> King::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            Position p(position.x + dx, position.y + dy);
            if (p.isValid() && (!board.getPieceAt(p) || board.getPieceAt(p)->getColor() != getColor())) {
                Move m(position, p);
                if (isValidMove(board, m))
                    moves.push_back(m);
            }
        }
    }
    return moves;
}

bool King::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) {
        return false;
    }

    int dx = std::abs(move.to.x - move.from.x);
    int dy = std::abs(move.to.y - move.from.y);

    //  ороль может двигатьс€ только на одну клетку в любом направлении
    if (dx > 1 || dy > 1) {
        return false;
    }

    // ѕровер€ем, что король не ходит на клетку, наход€щуюс€ под атакой
    Board boardCopy = board; // »спользуем копирующий конструктор

    // ”дал€ем корол€ с начальной позиции, чтобы он не блокировал атаку
    boardCopy.removePiece(move.from);

    // ѕровер€ем, атакуетс€ ли целева€ позици€
    Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (boardCopy.isPositionAttacked(move.to, opponentColor)) {
        return false;
    }

    return isPositionEmptyOrEnemy(board, move.to);
}