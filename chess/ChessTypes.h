#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <string>
#include <cctype>
#include <stdexcept>

// Перечисление для цвета фигур
enum class Color {
    WHITE, // Белые
    BLACK  // Черные
};

// Перечисление для типов шахматных фигур
enum class PieceType {
    KING,    // Король
    QUEEN,   // Ферзь
    ROOK,    // Ладья
    BISHOP,  // Слон
    KNIGHT,  // Конь
    PAWN     // Пешка
};

// Структура для представления позиции на доске (x, y)
struct Position {
    int x = -1;
    int y = -1;

    Position() = default;

    Position(int x, int y) : x(x), y(y) {}

    // Конструктор из шахматной нотации (например, "e4")
    Position(const std::string& s) {
        if (s.length() == 2) {
            x = std::tolower(s[0]) - 'a';
            y = s[1] - '1';
        }
    }

    bool isValid() const {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    std::string toString() const {
        if (!isValid()) return "inv";
        return std::string(1, 'a' + x) + std::to_string(y + 1);
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

// Структура для представления хода (откуда и куда)
struct Move {
    Position from;
    Position to;
    // Для превращения пешки. Используем KING как флаг "нет превращения".
    PieceType promotionType = PieceType::KING; 

    Move() = default;
    Move(const Position& from, const Position& to, PieceType promo = PieceType::KING) 
        : from(from), to(to), promotionType(promo) {}

    std::string toString() const {
        std::string s = from.toString() + to.toString();
        if (promotionType != PieceType::KING) {
            switch(promotionType) {
                case PieceType::QUEEN: s += 'q'; break;
                case PieceType::ROOK: s += 'r'; break;
                case PieceType::BISHOP: s += 'b'; break;
                case PieceType::KNIGHT: s += 'n'; break;
                default: break;
            }
        }
        return s;
    }
};

#endif // CHESS_TYPES_H