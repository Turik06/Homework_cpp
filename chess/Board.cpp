#include "Board.h"
#include "ChessPieces.h"
#include <iostream>
#include <algorithm>

Board::Board() {
    clear();
}

Board::~Board() {
    clear();
}

Board::Board(const Board& other) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (other.board[i][j]) {
                board[i][j] = other.board[i][j]->clone();
            } else {
                board[i][j] = nullptr;
            }
        }
    }
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        clear();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (other.board[i][j]) {
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
            board[i][j].reset();
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
        board[position.x][position.y].reset();
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
    if (!piece) {
        return false;
    }

    // --- Обработка рокировки ---
    if (piece->getType() == PieceType::KING && std::abs(move.to.x - move.from.x) == 2) {
        // Короткая рокировка
        if (move.to.x > move.from.x) {
            // Перемещаем короля
            board[6][move.from.y] = std::move(board[4][move.from.y]);
            board[6][move.from.y]->setPosition(Position(6, move.from.y));
            board[6][move.from.y]->setHasMoved(true);
            // Перемещаем ладью
            board[5][move.from.y] = std::move(board[7][move.from.y]);
            board[5][move.from.y]->setPosition(Position(5, move.from.y));
            board[5][move.from.y]->setHasMoved(true);
        } 
        // Длинная рокировка
        else {
            // Перемещаем короля
            board[2][move.from.y] = std::move(board[4][move.from.y]);
            board[2][move.from.y]->setPosition(Position(2, move.from.y));
            board[2][move.from.y]->setHasMoved(true);
            // Перемещаем ладью
            board[3][move.from.y] = std::move(board[0][move.from.y]);
            board[3][move.from.y]->setPosition(Position(3, move.from.y));
            board[3][move.from.y]->setHasMoved(true);
        }
        return true;
    }
    
    // Обычный ход
    board[move.to.x][move.to.y] = std::move(board[move.from.x][move.from.y]);
    piece->setPosition(move.to);
    piece->setHasMoved(true);

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
            if (piece && piece->getColor() == color && piece->getType() == PieceType::KING) {
                return piece->getPosition();
            }
        }
    }
    return Position(-1, -1);
}

bool Board::isPositionAttacked(const Position& position, Color attackingColor) const {
    auto pieces = getPiecesByColor(attackingColor);
    for (auto piece : pieces) {
        if (piece->isValidMove(*this, Move(piece->getPosition(), position))) {
            return true;
        }
    }
    return false;
}

bool Board::isKingInCheck(Color color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) return false;
    return isPositionAttacked(kingPos, color == Color::WHITE ? Color::BLACK : Color::WHITE);
}

bool Board::isKingInCheckmate(Color color) const {
    if (!isKingInCheck(color)) {
        return false;
    }

    auto piecesToMove = getPiecesByColor(color);
    for (ChessPiece* piece : piecesToMove) {
        auto possibleMoves = piece->getPossibleMoves(*this);
        for (const Move& currentMove : possibleMoves) {
            Board testBoard = *this;
            testBoard.makeMove(currentMove);
            if (!testBoard.isKingInCheck(color)) {
                return false;
            }
        }
    }
    return true;
}

void Board::print() const {
    std::cout << "  a b c d e f g h\n";
    std::cout << " +-----------------+\n";
    for (int j = 7; j >= 0; j--) {
        std::cout << j + 1 << "| ";
        for (int i = 0; i < 8; i++) {
            ChessPiece* piece = board[i][j].get();
            std::cout << (piece ? piece->getSymbol() : '.') << " ";
        }
        std::cout << "|\n";
    }
    std::cout << " +-----------------+\n";
}