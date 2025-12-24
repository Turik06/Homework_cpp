#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <string>
#include <cctype>
#include <stdexcept>

enum class Color { WHITE, BLACK };
enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

struct Position {
    int x = -1, y = -1;

    Position() = default;
    Position(int x, int y) : x(x), y(y) {}
    Position(const std::string& pos) {
        if (pos.length() >= 2) {
            x = std::tolower(pos[0]) - 'a';
            y = pos[1] - '1';
        }
    }

    bool isValid() const {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Position& other) const { return !(*this == other); }

    std::string toString() const {
        if (!isValid()) return "invalid";
        return std::string(1, 'a' + x) + std::to_string(y + 1);
    }
};

struct Move {
    Position from;
    Position to;
    PieceType promotionType = PieceType::KING; // KING означает "нет превращения"

    Move() = default;
    Move(Position from, Position to, PieceType promo = PieceType::KING)
        : from(from), to(to), promotionType(promo) {}

    std::string toString() const {
        std::string s = from.toString() + to.toString();
        if (promotionType != PieceType::KING) {
            switch (promotionType) {
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