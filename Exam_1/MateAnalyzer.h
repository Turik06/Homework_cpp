// MateAnalyzer.h
#ifndef MATE_ANALYZER_H
#define MATE_ANALYZER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "Board.h"
#include "ChessTypes.h"

// ��������� ��� �������� ���������� � ������� ������������
struct TranspositionEntry {
    int depth;      // ������� ������
    int score;      // ������ �������
    int flag;       // ��� ������: ������, ������� �������, ������ �������
    Move bestMove;  // ������ ���
};

// ����� ��� ������� ������������
enum NodeType {
    EXACT = 0,      // ������ ������
    ALPHA = 1,      // ������� �������
    BETA = 2        // ������ �������
};

class MateAnalyzer {
public:
    MateAnalyzer();

    // ��������� ����������� ���������� ���� � �������� ���������� �����
    bool canMateInNMoves(const Board& board, Color attackingColor, int maxMoves) const;

    // ���������� �������� ������������������ �����
    std::vector<Move> getMatingSequence(const Board& board, Color attackingColor, int maxMoves) const;

    // ��������� ����������� ���������� ���� � ����������� �����������
    bool canMateInNMovesIterativeDeepening(const Board& board, Color attackingColor, int maxMoves) const;

private:
    // ���������� Minimax � Alpha-Beta ����������
    int alphaBeta(Board& board, int depth, int alpha, int beta, bool maximizingPlayer,
        std::vector<Move>& pv, bool rootNode) const;

    // ���������, �������� �� ������ �������
    bool isMateScore(int score) const;

    // ��������� ��� ������� ��� ������� ������������
    std::string calculatePositionHash(const Board& board) const;

    // ������� ������������ - mutable ��� ����������� � const �������
    mutable std::unordered_map<std::string, TranspositionEntry> transpositionTable;

    // ���������� ���� ��� ������� � ������������ �������
    std::vector<Move> generateOrderedMoves(const Board& board, Color color) const;

    // ������������ ������� ������
    static const int MAX_DEPTH = 8; // 4 ���� = 8 ���������

    // ������ ��� ����
    static const int MATE_VALUE = 10000;

    // ������� ������������������ ����� ��� ���� - mutable ��� ����������� � const �������
    mutable std::vector<Move> currentMatingSequence;
};

#endif // MATE_ANALYZER_H
