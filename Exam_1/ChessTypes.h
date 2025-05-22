#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <string>

enum class Color { WHITE, BLACK };
enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

struct Position {
    int x; // 0-7, 'a'-'h'
    int y; // 0-7, '1'-'8'
    Position() : x(0), y(0) {}
    Position(int x, int y) : x(x), y(y) {}
    Position(const std::string& algebraic) {
        if (algebraic.length() >= 2) {
            x = std::tolower(algebraic[0]) - 'a';
            y = algebraic[1] - '1';
        }
    }
    bool isValid() const {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }
    std::string toAlgebraic() const {
        return std::string(1, 'a' + x) + std::string(1, '1' + y);
    }
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

struct Move {
    Position from;
    Position to;
    Move() {}
    Move(const Position& f, const Position& t) : from(f), to(t) {}
    std::string toString() const {
        return from.toAlgebraic() + "-" + to.toAlgebraic();
    }
};

#endif // CHESS_TYPES_H
