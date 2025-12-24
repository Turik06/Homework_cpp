#include "ChessPieces.h"
#include "Board.h"
#include <cmath>

// --- King ---
std::vector<Move> King::getPossibleMoves(const Board& board_ref) const {
    std::vector<Move> moves;
    // Обычные ходы короля
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            Position targetPos(position.x + dx, position.y + dy);
            if (targetPos.isValid() && isPositionEmptyOrEnemy(board_ref, targetPos)) {
                Board tempBoard = board_ref;
                tempBoard.makeMove(Move(position, targetPos));
                if (!tempBoard.isKingInCheck(this->color)) {
                    moves.push_back(Move(position, targetPos));
                }
            }
        }
    }

    // --- Логика рокировки ---
    if (!hasMoved && !board_ref.isKingInCheck(color)) {
        // Короткая рокировка (Kingside)
        ChessPiece* rookKingside = board_ref.getPieceAt(Position(7, position.y));
        if (rookKingside && rookKingside->getType() == PieceType::ROOK && !rookKingside->getHasMoved()) {
            if (board_ref.getPieceAt(Position(5, position.y)) == nullptr &&
                board_ref.getPieceAt(Position(6, position.y)) == nullptr) {
                if (!board_ref.isPositionAttacked(Position(5, position.y), color == Color::WHITE ? Color::BLACK : Color::WHITE) &&
                    !board_ref.isPositionAttacked(Position(6, position.y), color == Color::WHITE ? Color::BLACK : Color::WHITE)) {
                    moves.push_back(Move(position, Position(6, position.y)));
                }
            }
        }

        // Длинная рокировка (Queenside)
        ChessPiece* rookQueenside = board_ref.getPieceAt(Position(0, position.y));
        if (rookQueenside && rookQueenside->getType() == PieceType::ROOK && !rookQueenside->getHasMoved()) {
            if (board_ref.getPieceAt(Position(1, position.y)) == nullptr &&
                board_ref.getPieceAt(Position(2, position.y)) == nullptr &&
                board_ref.getPieceAt(Position(3, position.y)) == nullptr) {
                if (!board_ref.isPositionAttacked(Position(2, position.y), color == Color::WHITE ? Color::BLACK : Color::WHITE) &&
                    !board_ref.isPositionAttacked(Position(3, position.y), color == Color::WHITE ? Color::BLACK : Color::WHITE)) {
                    moves.push_back(Move(position, Position(2, position.y)));
                }
            }
        }
    }
    return moves;
}

bool King::isValidMove(const Board& board, const Move& move) const {
    // Проверка на рокировку
    if (!hasMoved && std::abs(move.to.x - move.from.x) == 2 && move.to.y == move.from.y) {
        // Валидность рокировки проверяется в getPossibleMoves, здесь просто разрешаем ход
        return true;
    }
    if (move.from != position) return false;
    int dx = std::abs(move.to.x - move.from.x);
    int dy = std::abs(move.to.y - move.from.y);
    if (dx > 1 || dy > 1) return false;
    return isPositionEmptyOrEnemy(board, move.to);
}

// --- Queen ---
std::vector<Move> Queen::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    const int dx_options[] = { 0, 1, 0, -1, 1, 1, -1, -1 };
    const int dy_options[] = { 1, 0, -1, 0, 1, -1, -1, 1 };

    for (int i = 0; i < 8; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx_options[i] * step, position.y + dy_options[i] * step);
            if (!newPos.isValid()) break;

            ChessPiece* target = board.getPieceAt(newPos);
            if (target == nullptr) {
                moves.push_back(Move(position, newPos));
            } else {
                if (target->getColor() != this->color) {
                    moves.push_back(Move(position, newPos));
                }
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
    if (dx == 0 && dy == 0) return false;
    if (dx != 0 && dy != 0 && std::abs(dx) != std::abs(dy)) return false;

    int stepX = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
    int stepY = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);

    Position checkPos(move.from.x + stepX, move.from.y + stepY);
    while (checkPos != move.to) {
        if (!checkPos.isValid() || board.getPieceAt(checkPos) != nullptr) return false;
        checkPos.x += stepX;
        checkPos.y += stepY;
    }
    return isPositionEmptyOrEnemy(board, move.to);
}

// --- Rook ---
std::vector<Move> Rook::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    const int dx_options[] = { 0, 1, 0, -1 };
    const int dy_options[] = { 1, 0, -1, 0 };

    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx_options[i] * step, position.y + dy_options[i] * step);
            if (!newPos.isValid()) break;
            ChessPiece* target = board.getPieceAt(newPos);
            if (target == nullptr) {
                moves.push_back(Move(position, newPos));
            } else {
                if (target->getColor() != this->color) {
                    moves.push_back(Move(position, newPos));
                }
                break;
            }
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

    int stepX = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
    int stepY = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);

    Position checkPos(move.from.x + stepX, move.from.y + stepY);
    while (checkPos != move.to) {
        if (!checkPos.isValid() || board.getPieceAt(checkPos) != nullptr) return false;
        checkPos.x += stepX;
        checkPos.y += stepY;
    }
    return isPositionEmptyOrEnemy(board, move.to);
}

// --- Bishop ---
std::vector<Move> Bishop::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    const int dx_options[] = { 1, 1, -1, -1 };
    const int dy_options[] = { 1, -1, -1, 1 };

    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx_options[i] * step, position.y + dy_options[i] * step);
            if (!newPos.isValid()) break;
            ChessPiece* target = board.getPieceAt(newPos);
            if (target == nullptr) {
                moves.push_back(Move(position, newPos));
            } else {
                if (target->getColor() != this->color) {
                    moves.push_back(Move(position, newPos));
                }
                break;
            }
        }
    }
    return moves;
}

bool Bishop::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) return false;
    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;
    if (std::abs(dx) != std::abs(dy) || dx == 0) return false;

    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    Position checkPos(move.from.x + stepX, move.from.y + stepY);
    while (checkPos != move.to) {
        if (!checkPos.isValid() || board.getPieceAt(checkPos) != nullptr) return false;
        checkPos.x += stepX;
        checkPos.y += stepY;
    }
    return isPositionEmptyOrEnemy(board, move.to);
}

// --- Knight ---
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
    bool isLMove = (dx_abs == 1 && dy_abs == 2) || (dx_abs == 2 && dy_abs == 1);
    return isLMove && isPositionEmptyOrEnemy(board, move.to);
}

// --- Pawn ---
std::vector<Move> Pawn::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;
    int forward = (color == Color::WHITE) ? 1 : -1;
    int promotionRank = (color == Color::WHITE) ? 7 : 0;

    // Ход на одну клетку вперед
    Position oneStep(position.x, position.y + forward);
    if (oneStep.isValid() && isPositionEmpty(board, oneStep)) {
        if (oneStep.y == promotionRank) {
            // --- Логика превращения ---
            moves.push_back(Move(position, oneStep, PieceType::QUEEN));
            moves.push_back(Move(position, oneStep, PieceType::ROOK));
            moves.push_back(Move(position, oneStep, PieceType::BISHOP));
            moves.push_back(Move(position, oneStep, PieceType::KNIGHT));
        } else {
            moves.push_back(Move(position, oneStep));
        }

        // Двойной ход с начальной позиции
        bool isStartingRank = (color == Color::WHITE && position.y == 1) || (color == Color::BLACK && position.y == 6);
        if (isStartingRank && isPositionEmpty(board, oneStep)) { // Проверяем, что и первая клетка свободна
            Position twoSteps(position.x, position.y + 2 * forward);
            if (twoSteps.isValid() && isPositionEmpty(board, twoSteps)) {
                moves.push_back(Move(position, twoSteps));
            }
        }
    }

    // Взятие по диагонали
    int capture_dx[] = { -1, 1 };
    for (int dx_val : capture_dx) {
        Position capturePos(position.x + dx_val, position.y + forward);
        if (capturePos.isValid() && canCaptureDiagonally(board, capturePos)) {
             if (capturePos.y == promotionRank) {
                // --- Взятие с превращением ---
                moves.push_back(Move(position, capturePos, PieceType::QUEEN));
                moves.push_back(Move(position, capturePos, PieceType::ROOK));
                moves.push_back(Move(position, capturePos, PieceType::BISHOP));
                moves.push_back(Move(position, capturePos, PieceType::KNIGHT));
            } else {
                moves.push_back(Move(position, capturePos));
            }
        }
    }
    return moves;
}


bool Pawn::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) return false;
    int forward = (color == Color::WHITE) ? 1 : -1;
    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;
    bool isStartingRank = (color == Color::WHITE && position.y == 1) || (color == Color::BLACK && position.y == 6);

    if (dx == 0 && dy == forward) {
        return isPositionEmpty(board, move.to);
    }
    if (isStartingRank && dx == 0 && dy == 2 * forward) {
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