#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <string>
#include <cctype> // Для std::tolower

// Перечисление цвета фигуры
enum class Color {
    WHITE, // Белые
    BLACK  // Черные
};

// Перечисление типов шахматных фигур
enum class PieceType {
    KING,    // Король
    QUEEN,   // Ферзь
    ROOK,    // Ладья
    BISHOP,  // Слон
    KNIGHT,  // Конь
    PAWN     // Пешка
};

// Структура для представления позиции на доске
struct Position {
    int x; // Столбец: 0 соответствует 'a', 1 — 'b', ..., 7 — 'h'
    int y; // Строка: 0 соответствует '1', ..., 7 — '8'

    // Конструктор по умолчанию (позиция a1)
    Position() : x(0), y(0) {}

    // Конструктор с координатами
    Position(int x, int y) : x(x), y(y) {}

    // Конструктор из алгебраической записи, например "e4"
    Position(const std::string& algebraic) {
        if (algebraic.length() >= 2) {
            x = std::tolower(algebraic[0]) - 'a';  // Преобразуем 'a'-'h' в 0-7
            y = algebraic[1] - '1';                // Преобразуем '1'-'8' в 0-7
        }
    }

    // Проверка, находится ли позиция в пределах доски
    bool isValid() const {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    // Преобразует координаты обратно в строку ("e4", "a1", и т.п.)
    std::string toAlgebraic() const {
        return std::string(1, 'a' + x) + std::string(1, '1' + y);
    }

    // Оператор сравнения на равенство
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    // Оператор сравнения на неравенство
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

// Структура для представления хода
struct Move {
    Position from; // Откуда
    Position to;   // Куда

    Move() {} // Конструктор по умолчанию

    // Конструктор из двух позиций
    Move(const Position& f, const Position& t) : from(f), to(t) {}

    // Строковое представление хода, например "e2-e4"
    std::string toString() const {
        return from.toAlgebraic() + "-" + to.toAlgebraic();
    }

    // Оператор "<" для использования в контейнерах (например, std::set)
    bool operator<(const Move& other) const {
        if (from.x != other.from.x) return from.x < other.from.x;
        if (from.y != other.from.y) return from.y < other.from.y;
        if (to.x != other.to.x) return to.x < other.to.x;
        return to.y < other.to.y;
    }
};

#endif // CHESS_TYPES_H
