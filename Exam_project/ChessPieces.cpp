#include "ChessPieces.h"
#include "Board.h"
#include <cmath>

std::vector<Move> King::getPossibleMoves(const Board& board_ref) const {
    std::vector<Move> moves;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            Position targetPos(position.x + dx, position.y + dy);
            if (targetPos.isValid() && isPositionEmptyOrEnemy(board_ref, targetPos)) {
                Color opponentColor = (this->color == Color::WHITE) ? Color::BLACK : Color::WHITE;
                if (!board_ref.isPositionAttacked(targetPos, opponentColor)) {
                    moves.push_back(Move(position, targetPos));
                }
            }
        }
    }
    return moves;
}

bool King::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) return false;
    int dx = std::abs(move.to.x - move.from.x);
    int dy = std::abs(move.to.y - move.from.y);
    if (dx > 1 || dy > 1) return false;
    return isPositionEmptyOrEnemy(board, move.to);
}

std::vector<Move> Queen::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    const int dx_options[] = { 0, 1, 0, -1, 1, 1, -1, -1 };
    const int dy_options[] = { 1, 0, -1, 0, 1, -1, -1, 1 };

    for (int i = 0; i < 8; i++) {
        int dx = dx_options[i];
        int dy = dy_options[i];
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx * step, position.y + dy * step);
            if (!newPos.isValid()) break;

            ChessPiece* target = board.getPieceAt(newPos);
            if (target == nullptr) {
                moves.push_back(Move(position, newPos));
            }
            else if (target->getColor() != this->getColor()) {
                moves.push_back(Move(position, newPos));
                break;
            }
            else {
                break;
            }
        }
    }
    return moves;
}

bool Queen::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) return false;

    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    if (dx != 0 && dy != 0 && std::abs(dx) != std::abs(dy)) return false;
    if (dx == 0 && dy == 0) return false;

    int stepX = (dx != 0) ? (dx > 0 ? 1 : -1) : 0;
    int stepY = (dy != 0) ? (dy > 0 ? 1 : -1) : 0;

    Position checkPos = move.from;
    checkPos.x += stepX;
    checkPos.y += stepY;
    while (checkPos != move.to) {
        if (!checkPos.isValid()) return false;
        if (board.getPieceAt(checkPos) != nullptr) return false;
        checkPos.x += stepX;
        checkPos.y += stepY;
    }

    return isPositionEmptyOrEnemy(board, move.to);
}

std::vector<Move> Rook::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    const int dx_options[] = { 0, 1, 0, -1 };
    const int dy_options[] = { 1, 0, -1, 0 };

    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx_options[i] * step, position.y + dy_options[i] * step);
            if (!newPos.isValid()) break;

            if (isPositionEmptyOrEnemy(board, newPos)) {
                moves.push_back(Move(position, newPos));
                if (board.getPieceAt(newPos) != nullptr) break;
            }
            else break;
        }
    }
    return moves;
}

bool Rook::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) return false;
    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    if (dx != 0 && dy != 0) return false;
    if (dx == 0 && dy == 0) return false;

    int stepX = (dx != 0) ? (dx > 0 ? 1 : -1) : 0;
    int stepY = (dy != 0) ? (dy > 0 ? 1 : -1) : 0;

    Position checkPos = move.from;
    checkPos.x += stepX;
    checkPos.y += stepY;
    while (checkPos != move.to) {
        if (!checkPos.isValid()) return false;
        if (board.getPieceAt(checkPos) != nullptr) return false;
        checkPos.x += stepX;
        checkPos.y += stepY;
    }

    return isPositionEmptyOrEnemy(board, move.to);
}

std::vector<Move> Bishop::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    const int dx_options[] = { 1, 1, -1, -1 };
    const int dy_options[] = { 1, -1, -1, 1 };

    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx_options[i] * step, position.y + dy_options[i] * step);
            if (!newPos.isValid()) break;

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
    if (move.from != position) return false;
    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    if (std::abs(dx) != std::abs(dy)) return false;
    if (dx == 0 && dy == 0) return false;

    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    Position checkPos = move.from;
    checkPos.x += stepX;
    checkPos.y += stepY;
    while (checkPos != move.to) {
        if (!checkPos.isValid()) return false;
        if (board.getPieceAt(checkPos) != nullptr) return false;
        checkPos.x += stepX;
        checkPos.y += stepY;
    }

    return isPositionEmptyOrEnemy(board, move.to);
}

std::vector<Move> Knight::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    const int dx_options[] = { 1, 2,  2,  1, -1, -2, -2, -1 };
    const int dy_options[] = { 2, 1, -1, -2, -2, -1,  1,  2 };

    for (int i = 0; i < 8; i++) {
        Position newPos(position.x + dx_options[i], position.y + dy_options[i]);
        if (newPos.isValid() && isPositionEmptyOrEnemy(board, newPos)) {
            moves.push_back(Move(position, newPos));
        }
    }
    return moves;
}

bool Knight::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) return false;
    int dx_abs = std::abs(move.to.x - move.from.x);
    int dy_abs = std::abs(move.to.y - move.from.y);
    return ((dx_abs == 1 && dy_abs == 2) || (dx_abs == 2 && dy_abs == 1)) &&
        isPositionEmptyOrEnemy(board, move.to);
}

std::vector<Move> Pawn::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    int forward = (color == Color::WHITE) ? 1 : -1;

    Position oneStep(position.x, position.y + forward);
    if (oneStep.isValid() && isPositionEmpty(board, oneStep)) {
        moves.push_back(Move(position, oneStep));
        if (canMoveForwardTwo()) {
            Position twoStep(position.x, position.y + 2 * forward);
            if (twoStep.isValid() && isPositionEmpty(board, twoStep)) {
                moves.push_back(Move(position, twoStep));
            }
        }
    }

    int capture_dx[] = { -1, 1 };
    for (int dx_val : capture_dx) {
        Position capturePos(position.x + dx_val, position.y + forward);
        if (capturePos.isValid() && canCaptureDiagonally(board, capturePos)) {
            moves.push_back(Move(position, capturePos));
        }
    }
    return moves;
}

bool Pawn::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) return false;
    int forward = (color == Color::WHITE) ? 1 : -1;
    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    if (dx == 0 && dy == forward) {
        return isPositionEmpty(board, move.to);
    }

    if (dx == 0 && dy == 2 * forward && canMoveForwardTwo()) {
        Position middlePos(move.from.x, move.from.y + forward);
        return isPositionEmpty(board, middlePos) && isPositionEmpty(board, move.to);
    }

    if (std::abs(dx) == 1 && dy == forward) {
        return canCaptureDiagonally(board, move.to);
    }

    return false;
}

bool Pawn::canCaptureDiagonally(const Board& board, const Position& target) const {
    ChessPiece* piece = board.getPieceAt(target);
    return piece != nullptr && piece->getColor() != this->color;
}
