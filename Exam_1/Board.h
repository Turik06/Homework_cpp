// Board.h
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include "ChessTypes.h"

class ChessPiece; // Forward declaration

class Board {
public:
    Board();
    Board(const Board& other); // Копирующий конструктор
    ~Board();

    Board& operator=(const Board& other); // Оператор присваивания

    // Очищает доску
    void clear();

    // Добавляет фигуру на доску
    void addPiece(std::unique_ptr<ChessPiece> piece);

    // Удаляет фигуру с доски
    void removePiece(const Position& position);

    // Возвращает указатель на фигуру в заданной позиции
    ChessPiece* getPieceAt(const Position& position) const;

    // Выполняет ход
    bool makeMove(const Move& move);

    // Возвращает все фигуры заданного цвета
    std::vector<ChessPiece*> getPiecesByColor(Color color) const;

    // Находит короля указанного цвета
    Position findKing(Color color) const;

    // Проверяет, находится ли король заданного цвета под шахом
    bool isKingInCheck(Color color) const;

    // Проверяет, находится ли позиция под атакой фигур указанного цвета
    bool isPositionAttacked(const Position& position, Color attackingColor) const;

    // Проверяет, находится ли король заданного цвета под матом
    bool isKingInCheckmate(Color color) const;

    // Проверяет, может ли указанный цвет поставить мат в 1 ход
    bool canCheckmateInOneMove(Color color) const;

    // Проверяет, может ли указанный цвет поставить мат в 2 хода
    bool canCheckmateInTwoMoves(Color color) const;

    // Возвращает количество фигур на доске
    int getPieceCount() const;

    // Возвращает количество доступных ходов для короля
    int getKingMobilityCount(Color color) const;

    // Подсчитывает количество атакующих фигур вблизи короля
    int countAttackersNearKing(Color attackingColor, const Position& kingPos) const;

    // Выводит текущее состояние доски
    void print() const;

private:
    // Массив указателей на фигуры
    std::unique_ptr<ChessPiece> board[8][8];
};

#endif // BOARD_H
