// Queen.cpp
#include "Queen.h"
#include "Board.h"
#include <cmath>

Queen::Queen(Color color, Position position)
    : ChessPiece(color, position) {}

std::vector<Move> Queen::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;

    // ����� ����� ��������� ��� ����� � ����
    // �������� �� ����������� � ��������� (��� �����)
    const int dx1[] = { 0, 1, 0, -1 };
    const int dy1[] = { 1, 0, -1, 0 };

    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx1[i] * step, position.y + dy1[i] * step);
            if (!isValidPosition(newPos)) break;

            if (isPositionEmptyOrEnemy(board, newPos)) {
                moves.push_back(Move(position, newPos));

                // ���� �� ���� ����������� ������ ����������, ������ ���� ������
                if (board.getPieceAt(newPos) != nullptr) {
                    break;
                }
            }
            else {
                break;
            }
        }
    }

    // �������� �� ��������� (��� ����)
    const int dx2[] = { 1, 1, -1, -1 };
    const int dy2[] = { 1, -1, -1, 1 };

    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position newPos(position.x + dx2[i] * step, position.y + dy2[i] * step);
            if (!isValidPosition(newPos)) break;

            if (isPositionEmptyOrEnemy(board, newPos)) {
                moves.push_back(Move(position, newPos));

                // ���� �� ���� ����������� ������ ����������, ������ ���� ������
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

bool Queen::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) {
        return false;
    }

    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    // ����� ����� ��������� �� ������ ��� �� ���������
    if (dx != 0 && dy != 0 && std::abs(dx) != std::abs(dy)) {
        return false;
    }

    // ���������, ��� �� ����� �� ����
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
            return false; // �� ���� ���� ������
        }
    }

    return isPositionEmptyOrEnemy(board, move.to);
}

PieceType Queen::getType() const {
    return PieceType::QUEEN;
}

char Queen::getSymbol() const {
    return color == Color::WHITE ? 'Q' : 'q';
}

std::unique_ptr<ChessPiece> Queen::clone() const {
    return std::make_unique<Queen>(color, position);
}
