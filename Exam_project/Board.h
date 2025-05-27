#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include "ChessTypes.h"

class ChessPiece;

class Board {
public:
    // Конструкторы и деструктор
    Board();                            // Создание пустой доски
    Board(const Board& other);          // Копирующий конструктор
    ~Board();                           // Деструктор

    // Оператор присваивания
    Board& operator=(const Board& other);

    // Очистка доски
    void clear();

    // Работа с фигурами
    void addPiece(std::unique_ptr<ChessPiece> piece);      // Добавление фигуры
    void removePiece(const Position& position);            // Удаление фигуры
    ChessPiece* getPieceAt(const Position& position) const; // Получить фигуру по позиции

    // Ходы
    bool makeMove(const Move& move);                       // Совершить ход

    // Поиск фигур и проверок
    std::vector<ChessPiece*> getPiecesByColor(Color color) const; // Все фигуры цвета
    Position findKing(Color color) const;                          // Позиция короля
    bool isKingInCheck(Color color) const;                         // Шах королю
    bool isPositionAttacked(const Position& position, Color attackingColor) const; // Атакована ли позиция
    bool isKingInCheckmate(Color color) const;                     // Мат

    // Вывод доски в консоль
    void print() const;

private:
    // 8x8 доска с уникальными фигурами
    std::unique_ptr<ChessPiece> board[8][8];
};

#endif // BOARD_H
