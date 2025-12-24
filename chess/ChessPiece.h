#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "ChessTypes.h"
#include <vector>
#include <memory>

class Board; // Предварительное объявление для избежания циклической зависимости

// Абстрактный базовый класс для всех шахматных фигур
class ChessPiece {
protected:
    Color color;
    Position position;
    PieceType type;
    bool hasMoved; // Флаг для отслеживания, двигалась ли фигура

public:
    ChessPiece(Color c, PieceType t, const Position& p) : color(c), type(t), position(p), hasMoved(false) {}
    virtual ~ChessPiece() = default;

    // Виртуальные методы, которые должны быть реализованы наследниками
    virtual std::vector<Move> getPossibleMoves(const Board& board) const = 0;
    virtual bool isValidMove(const Board& board, const Move& move) const = 0;
    virtual char getSymbol() const = 0;
    virtual std::unique_ptr<ChessPiece> clone() const = 0;

    // Общие методы
    Color getColor() const { return color; }
    PieceType getType() const { return type; }
    Position getPosition() const { return position; }
    void setPosition(const Position& p) { position = p; }
    
    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    // Вспомогательные методы для проверки полей
    bool isPositionEmptyOrEnemy(const Board& board, const Position& p) const;
    bool isPositionEmpty(const Board& board, const Position& p) const;
};

#endif // CHESS_PIECE_H