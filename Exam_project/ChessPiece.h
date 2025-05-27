#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "ChessTypes.h"
#include <vector>
#include <memory>

class Board;

// Абстрактный базовый класс для всех шахматных фигур
class ChessPiece {
public:
    // Конструктор с цветом и позицией
    ChessPiece(Color c, Position p) : color(c), position(p) {}
    virtual ~ChessPiece() = default;

    // Геттеры и сеттеры
    Color getColor() const { return color; }            // Цвет фигуры
    Position getPosition() const { return position; }   // Текущая позиция
    void setPosition(const Position& p) { position = p; } // Установить позицию

    // Чисто виртуальные методы (переопределяются в подклассах)
    virtual std::vector<Move> getPossibleMoves(const Board& board) const = 0; // Все допустимые ходы
    virtual bool isValidMove(const Board& board, const Move& move) const = 0; // Проверка допустимости хода
    virtual PieceType getType() const = 0;    // Тип фигуры
    virtual char getSymbol() const = 0;       // Символ фигуры
    virtual std::unique_ptr<ChessPiece> clone() const = 0; // Клонирование фигуры

protected:
    Color color;         // Цвет фигуры
    Position position;   // Позиция на доске

    // Вспомогательные проверки для ходов
    bool isValidPosition(const Position& p) const { return p.isValid(); }      // Проверка допустимости позиции
    bool isPositionEmptyOrEnemy(const Board& board, const Position& p) const;  // Пустая или вражеская клетка
    bool isPositionEmpty(const Board& board, const Position& p) const;         // Пустая клетка
};

#endif // CHESS_PIECE_H
