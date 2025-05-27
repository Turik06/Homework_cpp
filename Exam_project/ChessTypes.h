#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <string>
#include <cctype> // ��� std::tolower

// ������������ ����� ������
enum class Color {
    WHITE, // �����
    BLACK  // ������
};

// ������������ ����� ��������� �����
enum class PieceType {
    KING,    // ������
    QUEEN,   // �����
    ROOK,    // �����
    BISHOP,  // ����
    KNIGHT,  // ����
    PAWN     // �����
};

// ��������� ��� ������������� ������� �� �����
struct Position {
    int x; // �������: 0 ������������� 'a', 1 � 'b', ..., 7 � 'h'
    int y; // ������: 0 ������������� '1', ..., 7 � '8'

    // ����������� �� ��������� (������� a1)
    Position() : x(0), y(0) {}

    // ����������� � ������������
    Position(int x, int y) : x(x), y(y) {}

    // ����������� �� �������������� ������, �������� "e4"
    Position(const std::string& algebraic) {
        if (algebraic.length() >= 2) {
            x = std::tolower(algebraic[0]) - 'a';  // ����������� 'a'-'h' � 0-7
            y = algebraic[1] - '1';                // ����������� '1'-'8' � 0-7
        }
    }

    // ��������, ��������� �� ������� � �������� �����
    bool isValid() const {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    // ����������� ���������� ������� � ������ ("e4", "a1", � �.�.)
    std::string toAlgebraic() const {
        return std::string(1, 'a' + x) + std::string(1, '1' + y);
    }

    // �������� ��������� �� ���������
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    // �������� ��������� �� �����������
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

// ��������� ��� ������������� ����
struct Move {
    Position from; // ������
    Position to;   // ����

    Move() {} // ����������� �� ���������

    // ����������� �� ���� �������
    Move(const Position& f, const Position& t) : from(f), to(t) {}

    // ��������� ������������� ����, �������� "e2-e4"
    std::string toString() const {
        return from.toAlgebraic() + "-" + to.toAlgebraic();
    }

    // �������� "<" ��� ������������� � ����������� (��������, std::set)
    bool operator<(const Move& other) const {
        if (from.x != other.from.x) return from.x < other.from.x;
        if (from.y != other.from.y) return from.y < other.from.y;
        if (to.x != other.to.x) return to.x < other.to.x;
        return to.y < other.to.y;
    }
};

#endif // CHESS_TYPES_H
