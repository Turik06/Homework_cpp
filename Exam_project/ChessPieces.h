#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "ChessPiece.h"

// === КОРОЛЬ ===
class King : public ChessPiece {
public:
    King(Color c, Position p) : ChessPiece(c, p) {}

    // Возвращает допустимые ходы для короля (на одну клетку в любом направлении)
    std::vector<Move> getPossibleMoves(const Board& board) const override;

    // Проверяет допустимость конкретного хода
    bool isValidMove(const Board& board, const Move& move) const override;

    // Тип фигуры
    PieceType getType() const override { return PieceType::KING; }

    // Символ фигуры для вывода
    char getSymbol() const override { return color == Color::WHITE ? 'K' : 'k'; }

    // Создает копию фигуры
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<King>(*this);
    }
};

// === ФЕРЗЬ ===
class Queen : public ChessPiece {
public:
    Queen(Color c, Position p) : ChessPiece(c, p) {}

    // Ходы как у ладьи и слона вместе
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::QUEEN; }
    char getSymbol() const override { return color == Color::WHITE ? 'Q' : 'q'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Queen>(*this);
    }
};

// === ЛАДЬЯ ===
class Rook : public ChessPiece {
public:
    Rook(Color c, Position p) : ChessPiece(c, p) {}

    // Ходы по вертикали и горизонтали
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::ROOK; }
    char getSymbol() const override { return color == Color::WHITE ? 'R' : 'r'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Rook>(*this);
    }
};

// === СЛОН ===
class Bishop : public ChessPiece {
public:
    Bishop(Color c, Position p) : ChessPiece(c, p) {}

    // Диагональные ходы
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::BISHOP; }
    char getSymbol() const override { return color == Color::WHITE ? 'B' : 'b'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Bishop>(*this);
    }
};

// === КОНЬ ===
class Knight : public ChessPiece {
public:
    Knight(Color c, Position p) : ChessPiece(c, p) {}

    // L-образные ходы (2+1 клетки)
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::KNIGHT; }
    char getSymbol() const override { return color == Color::WHITE ? 'N' : 'n'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Knight>(*this);
    }
};

// === ПЕШКА ===
class Pawn : public ChessPiece {
public:
    Pawn(Color c, Position p) : ChessPiece(c, p) {}

    // Прямой ход на 1 или 2 клетки, захват по диагонали
    std::vector<Move> getPossibleMoves(const Board& board) const override;

    // Проверка конкретного хода
    bool isValidMove(const Board& board, const Move& move) const override;

    // Тип фигуры
    PieceType getType() const override { return PieceType::PAWN; }

    // Символ
    char getSymbol() const override { return color == Color::WHITE ? 'P' : 'p'; }

    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Pawn>(*this);
    }

private:
    // Может ли пешка сделать начальный двойной ход
    bool canMoveForwardTwo() const {
        return (color == Color::WHITE && position.y == 1) ||
            (color == Color::BLACK && position.y == 6);
    }

    // Проверяет возможность взятия по диагонали
    bool canCaptureDiagonally(const Board& board, const Position& target) const;
};

#endif // CHESS_PIECES_H
