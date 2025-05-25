#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "ChessPiece.h"

class King : public ChessPiece {
public:
    King(Color c, Position p) : ChessPiece(c, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::KING; }
    char getSymbol() const override { return color == Color::WHITE ? 'K' : 'k'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<King>(*this);
    }
};

class Queen : public ChessPiece {
public:
    Queen(Color c, Position p) : ChessPiece(c, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::QUEEN; }
    char getSymbol() const override { return color == Color::WHITE ? 'Q' : 'q'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Queen>(*this);
    }
};

class Rook : public ChessPiece {
public:
    Rook(Color c, Position p) : ChessPiece(c, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::ROOK; }
    char getSymbol() const override { return color == Color::WHITE ? 'R' : 'r'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Rook>(*this);
    }
};

class Bishop : public ChessPiece {
public:
    Bishop(Color c, Position p) : ChessPiece(c, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::BISHOP; }
    char getSymbol() const override { return color == Color::WHITE ? 'B' : 'b'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Bishop>(*this);
    }
};

class Knight : public ChessPiece {
public:
    Knight(Color c, Position p) : ChessPiece(c, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::KNIGHT; }
    char getSymbol() const override { return color == Color::WHITE ? 'N' : 'n'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Knight>(*this);
    }
};

class Pawn : public ChessPiece {
public:
    Pawn(Color c, Position p) : ChessPiece(c, p) {}
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool isValidMove(const Board& board, const Move& move) const override;
    PieceType getType() const override { return PieceType::PAWN; }
    char getSymbol() const override { return color == Color::WHITE ? 'P' : 'p'; }
    std::unique_ptr<ChessPiece> clone() const override {
        return std::make_unique<Pawn>(*this);
    }

private:
    bool canMoveForwardTwo() const {
        return (color == Color::WHITE && position.y == 1) ||
            (color == Color::BLACK && position.y == 6);
    }
    bool canCaptureDiagonally(const Board& board, const Position& target) const;
};

#endif // CHESS_PIECES_H
