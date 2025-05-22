// Pawn.cpp
#include "Pawn.h"
#include "Board.h"
#include <cmath>

Pawn::Pawn(Color color, Position position)
    : ChessPiece(color, position) {}

std::vector<Move> Pawn::getPossibleMoves(const Board& board) const {
    std::vector<Move> moves;

    // ����������� �������� ������� �� ����� �����
    int forward = (color == Color::WHITE) ? 1 : -1;

    // ��� ����� �� 1 ������
    Position newPos(position.x, position.y + forward);
    if (isValidPosition(newPos) && isPositionEmpty(board, newPos)) {
        moves.push_back(Move(position, newPos));

        // ��� ����� �� 2 ������ (������ �� ��������� �������)
        if (canMoveForwardTwo(board)) {
            Position newPos2(position.x, position.y + 2 * forward);
            if (isPositionEmpty(board, newPos2)) {
                moves.push_back(Move(position, newPos2));
            }
        }
    }

    // ��� �� ��������� ��� ������ ������
    Position capturePos1(position.x + 1, position.y + forward);
    if (isValidPosition(capturePos1) && canCaptureDiagonally(board, capturePos1)) {
        moves.push_back(Move(position, capturePos1));
    }

    Position capturePos2(position.x - 1, position.y + forward);
    if (isValidPosition(capturePos2) && canCaptureDiagonally(board, capturePos2)) {
        moves.push_back(Move(position, capturePos2));
    }

    return moves;
}

bool Pawn::isValidMove(const Board& board, const Move& move) const {
    if (move.from != position) {
        return false;
    }

    // ����������� �������� ������� �� ����� �����
    int forward = (color == Color::WHITE) ? 1 : -1;

    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    // ��� ����� �� 1 ������
    if (dx == 0 && dy == forward) {
        return isPositionEmpty(board, move.to);
    }

    // ��� ����� �� 2 ������ (������ �� ��������� �������)
    if (dx == 0 && dy == 2 * forward && canMoveForwardTwo(board)) {
        Position middlePos(move.from.x, move.from.y + forward);
        return isPositionEmpty(board, middlePos) && isPositionEmpty(board, move.to);
    }

    // ��� �� ��������� ��� ������ ������
    if (std::abs(dx) == 1 && dy == forward) {
        return canCaptureDiagonally(board, move.to);
    }

    return false;
}

bool Pawn::canMoveForwardTwo(const Board& board) const {
    // ����� ����� ������ �� 2 ������ ������ �� ��������� �������
    if (color == Color::WHITE) {
        return position.y == 1; // ������ ������ �����
    }
    else {
        return position.y == 6; // ������ ������ ������
    }
}

bool Pawn::canCaptureDiagonally(const Board& board, const Position& target) const {
    ChessPiece* piece = board.getPieceAt(target);
    return piece != nullptr && piece->getColor() != color;
}

PieceType Pawn::getType() const {
    return PieceType::PAWN;
}

char Pawn::getSymbol() const {
    return color == Color::WHITE ? 'P' : 'p';
}

std::unique_ptr<ChessPiece> Pawn::clone() const {
    return std::make_unique<Pawn>(color, position);
}
