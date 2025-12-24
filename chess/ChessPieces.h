#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "ChessPiece.h"

// Макрос для упрощения создания методов clone()
#define CLONE_METHOD(CLASS_NAME) \
    std::unique_ptr<ChessPiece> clone() const override { \
        return std::make_unique<CLASS_NAME>(*this); \
    }

// === Король ===
class King : public ChessPiece {
public:
    King(Color c, const Position& p) : ChessPiece(c, PieceType::KING, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'K' : 'k'; }
    CLONE_METHOD(King)
};

// === Ферзь ===
class Queen : public ChessPiece {
public:
    Queen(Color c, const Position& p) : ChessPiece(c, PieceType::QUEEN, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'Q' : 'q'; }
    CLONE_METHOD(Queen)
};

// === Ладья ===
class Rook : public ChessPiece {
public:
    Rook(Color c, const Position& p) : ChessPiece(c, PieceType::ROOK, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'R' : 'r'; }
    CLONE_METHOD(Rook)
};

// === Слон ===
class Bishop : public ChessPiece {
public:
    Bishop(Color c, const Position& p) : ChessPiece(c, PieceType::BISHOP, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'B' : 'b'; }
    CLONE_METHOD(Bishop)
};

// === Конь ===
class Knight : public ChessPiece {
public:
    Knight(Color c, const Position& p) : ChessPiece(c, PieceType::KNIGHT, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'N' : 'n'; }
    CLONE_METHOD(Knight)
};

// === Пешка ===
class Pawn : public ChessPiece {
public:
    Pawn(Color c, const Position& p) : ChessPiece(c, PieceType::PAWN, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    char getSymbol() const override { return (color == Color::WHITE) ? 'P' : 'p'; }
    CLONE_METHOD(Pawn)
private:
    bool canCaptureDiagonally(const Board& board, const Position& target) const;
};

#endif // CHESS_PIECES_H