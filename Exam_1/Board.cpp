// Board.cpp
#include "Board.h"
#include "ChessPiece.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Board::Board() {
    clear();
}

Board::~Board() {
    clear();
}

// ���������� �����������
Board::Board(const Board& other) {
    clear(); // ������� ���� �����

    // �������� ��� ������ � ������ �����
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (other.board[i][j]) {
                // ���������� clone() ��� �������� �������� ����� ������
                board[i][j] = other.board[i][j]->clone();
            }
        }
    }
}

// �������� ������������
Board& Board::operator=(const Board& other) {
    if (this != &other) { // �������� �� ����������������
        clear(); // ������� ������� �����

        // �������� ��� ������ � ������ �����
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (other.board[i][j]) {
                    // ���������� clone() ��� �������� �������� ����� ������
                    board[i][j] = other.board[i][j]->clone();
                }
            }
        }
    }
    return *this;
}

void Board::clear() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }
}

void Board::addPiece(std::unique_ptr<ChessPiece> piece) {
    Position pos = piece->getPosition();
    if (pos.isValid()) {
        board[pos.x][pos.y] = std::move(piece);
    }
}

void Board::removePiece(const Position& position) {
    if (position.isValid()) {
        board[position.x][position.y] = nullptr;
    }
}

ChessPiece* Board::getPieceAt(const Position& position) const {
    if (position.isValid()) {
        return board[position.x][position.y].get();
    }
    return nullptr;
}

bool Board::makeMove(const Move& move) {
    ChessPiece* piece = getPieceAt(move.from);
    if (!piece || !piece->isValidMove(*this, move)) {
        return false;
    }

    // ������� ��������� ��������� ��� ������������ ������
    std::unique_ptr<ChessPiece> movedPiece = std::move(board[move.from.x][move.from.y]);

    // ������� ������ � ������� �������, ���� ��� ��� ����
    board[move.to.x][move.to.y] = nullptr;

    // ��������� ������� ������������� ������
    movedPiece->setPosition(move.to);

    // �������� ������ �� ����� �������
    board[move.to.x][move.to.y] = std::move(movedPiece);

    return true;
}

std::vector<ChessPiece*> Board::getPiecesByColor(Color color) const {
    std::vector<ChessPiece*> pieces;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board[i][j].get();
            if (piece && piece->getColor() == color) {
                pieces.push_back(piece);
            }
        }
    }

    return pieces;
}

Position Board::findKing(Color color) const {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board[i][j].get();
            if (piece && piece->getColor() == color &&
                piece->getType() == PieceType::KING) {
                return Position(i, j);
            }
        }
    }

    // ���� ������ �� ������, ���������� ���������� �������
    return Position(-1, -1);
}

bool Board::isPositionAttacked(const Position& position, Color attackingColor) const {
    // ��������� ��� ������ ���������� �����
    auto pieces = getPiecesByColor(attackingColor);

    for (auto piece : pieces) {
        // ��� ������ ������ ���������, ����� �� ��� ��������� ��������� �������
        Move move(piece->getPosition(), position);
        if (piece->isValidMove(*this, move)) {
            return true;
        }
    }

    return false;
}

bool Board::isKingInCheck(Color color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) {
        return false; // ������ ��� �� �����
    }

    // ���������, ��������� �� ������ �������� ���������������� �����
    return isPositionAttacked(kingPos, color == Color::WHITE ? Color::BLACK : Color::WHITE);
}

bool Board::isKingInCheckmate(Color color) const {
    // ���� ������ �� ��� �����, �� � ��� ����� �� ��� �����
    if (!isKingInCheck(color)) {
        return false;
    }

    // �������� ��� ������ ���������� �����
    auto pieces = getPiecesByColor(color);

    // ��������� ������ ������ � ������ ��������� ���
    for (auto piece : pieces) {
        auto moves = piece->getPossibleMoves(*this);

        for (const auto& move : moves) {
            // ������� ����� ����� ��� ��������� ����
            Board boardCopy = *this;

            // ��������� ��� �� �����
            boardCopy.makeMove(move);

            // ���������, ������� �� ������ ��� ����� ����� ����
            if (!boardCopy.isKingInCheck(color)) {
                return false; // ����� ���, ������� ������� �� ����
            }
        }
    }

    // ���� �� ���� ��� �� ������� �� ����, �� ��� ���
    return true;
}

bool Board::canCheckmateInOneMove(Color color) const {
    // �������� ��� ������ ���������� �����
    auto pieces = getPiecesByColor(color);

    // ��������� ������ ������ � ������ ��������� ���
    for (auto piece : pieces) {
        auto moves = piece->getPossibleMoves(*this);

        for (const auto& move : moves) {
            // ������� ����� ����� ��� ��������� ����
            Board boardCopy = *this;

            // ��������� ��� �� �����
            boardCopy.makeMove(move);

            // ���������, ������ �� ��� � ����
            if (boardCopy.isKingInCheckmate(color == Color::WHITE ? Color::BLACK : Color::WHITE)) {
                return true;
            }
        }
    }

    return false;
}

bool Board::canCheckmateInTwoMoves(Color color) const {
    // ������� ��������� ��� � 1 ���
    if (canCheckmateInOneMove(color)) {
        return true;
    }

    // �������� ��� ������ ���������� �����
    auto pieces = getPiecesByColor(color);

    // ��������� ������ ������ � ������ ��������� ���
    for (auto piece : pieces) {
        auto moves = piece->getPossibleMoves(*this);

        for (const auto& move : moves) {
            // ������� ����� ����� ��� ��������� ������� ����
            Board boardCopy = *this;

            // ��������� ������ ��� �� �����
            boardCopy.makeMove(move);

            // ���������, ����� �� ��������� ������� ���
            Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
            auto opponentPieces = boardCopy.getPiecesByColor(opponentColor);

            bool opponentCanMove = false;
            bool opponentCanAvoidMate = false;

            for (auto opponentPiece : opponentPieces) {
                auto opponentMoves = opponentPiece->getPossibleMoves(boardCopy);

                // ���� � ���������� ���� ����
                if (!opponentMoves.empty()) {
                    opponentCanMove = true;

                    for (const auto& opponentMove : opponentMoves) {
                        // ������� ��� ���� ����� ��� ��������� ���� ����������
                        Board boardCopy2 = boardCopy;
                        boardCopy2.makeMove(opponentMove);

                        // ���������, ����� �� �� ��������� ��� ����� ������ ����������
                        if (!boardCopy2.canCheckmateInOneMove(color)) {
                            opponentCanAvoidMate = true;
                            break;
                        }
                    }

                    if (opponentCanAvoidMate) {
                        break;
                    }
                }
            }

            // ���� � ���������� ��� ����� ��� ��� ��� ���� ����� � ���� � ��������� ����
            if (!opponentCanMove || !opponentCanAvoidMate) {
                return true;
            }
        }
    }

    return false;
}

int Board::getPieceCount() const {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) {
                count++;
            }
        }
    }
    return count;
}

int Board::getKingMobilityCount(Color color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) {
        return 0;
    }

    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            Position newPos(kingPos.x + dx, kingPos.y + dy);
            if (newPos.isValid()) {
                ChessPiece* piece = getPieceAt(newPos);
                if (!piece || piece->getColor() != color) {
                    // ���������, �� ��������� �� ������� ��� ������
                    if (!isPositionAttacked(newPos, color == Color::WHITE ? Color::BLACK : Color::WHITE)) {
                        count++;
                    }
                }
            }
        }
    }

    return count;
}

int Board::countAttackersNearKing(Color attackingColor, const Position& kingPos) const {
    int count = 0;
    auto attackers = getPiecesByColor(attackingColor);

    for (auto attacker : attackers) {
        Position pos = attacker->getPosition();
        int dx = std::abs(pos.x - kingPos.x);
        int dy = std::abs(pos.y - kingPos.y);

        // ������� ������ � ������� 2 ������ �� ������
        if (dx <= 2 && dy <= 2) {
            count++;
        }
    }

    return count;
}

void Board::print() const {
    std::cout << "  a b c d e f g h\n";
    std::cout << " +-----------------+\n";

    for (int j = 7; j >= 0; j--) {
        std::cout << j + 1 << "| ";

        for (int i = 0; i < 8; i++) {
            ChessPiece* piece = board[i][j].get();
            if (piece) {
                std::cout << piece->getSymbol() << " ";
            }
            else {
                std::cout << ". ";
            }
        }

        std::cout << "|\n";
    }

    std::cout << " +-----------------+\n";
}
