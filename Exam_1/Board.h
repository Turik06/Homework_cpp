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
    Board(const Board& other); // ���������� �����������
    ~Board();

    Board& operator=(const Board& other); // �������� ������������

    // ������� �����
    void clear();

    // ��������� ������ �� �����
    void addPiece(std::unique_ptr<ChessPiece> piece);

    // ������� ������ � �����
    void removePiece(const Position& position);

    // ���������� ��������� �� ������ � �������� �������
    ChessPiece* getPieceAt(const Position& position) const;

    // ��������� ���
    bool makeMove(const Move& move);

    // ���������� ��� ������ ��������� �����
    std::vector<ChessPiece*> getPiecesByColor(Color color) const;

    // ������� ������ ���������� �����
    Position findKing(Color color) const;

    // ���������, ��������� �� ������ ��������� ����� ��� �����
    bool isKingInCheck(Color color) const;

    // ���������, ��������� �� ������� ��� ������ ����� ���������� �����
    bool isPositionAttacked(const Position& position, Color attackingColor) const;

    // ���������, ��������� �� ������ ��������� ����� ��� �����
    bool isKingInCheckmate(Color color) const;

    // ���������, ����� �� ��������� ���� ��������� ��� � 1 ���
    bool canCheckmateInOneMove(Color color) const;

    // ���������, ����� �� ��������� ���� ��������� ��� � 2 ����
    bool canCheckmateInTwoMoves(Color color) const;

    // ���������� ���������� ����� �� �����
    int getPieceCount() const;

    // ���������� ���������� ��������� ����� ��� ������
    int getKingMobilityCount(Color color) const;

    // ������������ ���������� ��������� ����� ������ ������
    int countAttackersNearKing(Color attackingColor, const Position& kingPos) const;

    // ������� ������� ��������� �����
    void print() const;

private:
    // ������ ���������� �� ������
    std::unique_ptr<ChessPiece> board[8][8];
};

#endif // BOARD_H
