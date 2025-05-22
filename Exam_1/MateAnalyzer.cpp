// MateAnalyzer.cpp
#include "MateAnalyzer.h"
#include "ChessPiece.h" // ���������� ��� ����������� ������ C2027
#include <algorithm>
#include <sstream>

MateAnalyzer::MateAnalyzer() {
}

bool MateAnalyzer::canMateInNMoves(const Board& board, Color attackingColor, int maxMoves) const {
    // ������� ����� �����, ��� ��� �� ����� � ��������������
    Board boardCopy = board;

    // ��� �������� ���� � N ����� ����� ������ �� ������� 2*N (���������)
    int depth = 2 * maxMoves;

    // �������������� ���������� ��� ������
    std::vector<Move> principalVariation;
    int alpha = -MATE_VALUE - 1;
    int beta = MATE_VALUE + 1;

    // ��������� �����
    int score = alphaBeta(boardCopy, depth, alpha, beta,
        attackingColor == Color::WHITE, principalVariation, true);

    // ���� ������ ���, ��������� ������������������ �����
    if (isMateScore(score)) {
        currentMatingSequence = principalVariation;
        return true;
    }

    return false;
}

std::vector<Move> MateAnalyzer::getMatingSequence(const Board& board, Color attackingColor, int maxMoves) const {
    // ���� �� ��� �� ���������������� �������, ������� ��� ������
    if (currentMatingSequence.empty()) {
        canMateInNMoves(board, attackingColor, maxMoves);
    }

    return currentMatingSequence;
}

bool MateAnalyzer::canMateInNMovesIterativeDeepening(const Board& board, Color attackingColor, int maxMoves) const {
    // ������� ������� ������������ ��� ������ �������
    transpositionTable.clear();

    // �������� � ������� 1 � ���������� ����������� �� ������������
    for (int currentDepth = 1; currentDepth <= maxMoves; currentDepth++) {
        if (canMateInNMoves(board, attackingColor, currentDepth)) {
            return true;
        }
    }

    return false;
}

int MateAnalyzer::alphaBeta(Board& board, int depth, int alpha, int beta, bool maximizingPlayer,
    std::vector<Move>& pv, bool rootNode) const {
    // ������� ������: ���������� ������������ ������� ��� ������������ �������
    if (depth == 0) {
        // ��� ������� ���� �� ���������� ����������� ������ �������,
        // � ������ ���������, �������� �� ������� ����� ��� ���
        Color defendingColor = maximizingPlayer ? Color::BLACK : Color::WHITE;
        Color attackingColor = maximizingPlayer ? Color::WHITE : Color::BLACK;

        if (board.isKingInCheckmate(defendingColor)) {
            return maximizingPlayer ? MATE_VALUE + depth : -MATE_VALUE - depth;
        }

        // ������ � ������ ���������� �� ����
        if (board.isKingInCheck(defendingColor)) {
            int score = 500; // ������� ������ ��� ����

            // ��������� ������ �� ������������ ����������� ������
            int kingMoves = board.getKingMobilityCount(defendingColor);
            score += (8 - kingMoves) * 50; // ��� ������ ����� � ������, ��� �����

            // ��������� ���������� ��������� ����� ������ ������
            Position kingPos = board.findKing(defendingColor);
            int attackersNearKing = board.countAttackersNearKing(attackingColor, kingPos);
            score += attackersNearKing * 100;

            return maximizingPlayer ? score : -score;
        }

        // � ������ ���� ��� ������� ������� ���������� 0 (�� ���)
        return 0;
    }

    // ��������� ��� �������
    std::string positionHash = calculatePositionHash(board);

    // ���������, ���� �� ������� � ������� ������������
    if (transpositionTable.find(positionHash) != transpositionTable.end()) {
        TranspositionEntry entry = transpositionTable[positionHash];

        // ���������� ���������� �� �������, ���� ������� ������ ���� �����������
        if (entry.depth >= depth) {
            if (entry.flag == EXACT) {
                return entry.score;
            }
            else if (entry.flag == ALPHA && entry.score <= alpha) {
                return alpha;
            }
            else if (entry.flag == BETA && entry.score >= beta) {
                return beta;
            }
        }
    }

    // PV-�����: ��������� �������������� �������� ����� ��� �����������,
    // ��� �� ������ ������ ��������� ��� ������ �������
    int originalAlpha = alpha;

    // ������� PV ��� ����� ����
    std::vector<Move> currentPv;

    // ���������� ���� � ����������� �������
    Color currentColor = maximizingPlayer ? Color::WHITE : Color::BLACK;
    std::vector<Move> moves = generateOrderedMoves(board, currentColor);

    // ���� ��� �����, ��������� �� ��� ��� ���
    if (moves.empty()) {
        if (board.isKingInCheck(currentColor)) {
            // ���: ���������� -MATE_VALUE � ��������� �� �������
            return maximizingPlayer ? -MATE_VALUE - depth : MATE_VALUE + depth;
        }
        else {
            // ���: ���������� 0
            return 0;
        }
    }

    Move bestMove;
    int bestScore = maximizingPlayer ? -MATE_VALUE - 1 : MATE_VALUE + 1;

    // ��������� ��� ����
    for (const Move& move : moves) {
        // ��������� ������� ��������� �����
        Board originalBoard = board;

        // ������ ���
        if (!board.makeMove(move)) {
            continue; // ��� ����������
        }

        // ���������� ��������� ������
        std::vector<Move> childPv;
        int score = alphaBeta(board, depth - 1, alpha, beta, !maximizingPlayer, childPv, false);

        // ��������������� ��������� �����
        board = originalBoard;

        // ��������� ������ ���������
        if (maximizingPlayer) {
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
                currentPv.clear();
                currentPv.push_back(move);
                currentPv.insert(currentPv.end(), childPv.begin(), childPv.end());
            }
            alpha = std::max(alpha, bestScore);
        }
        else {
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
                currentPv.clear();
                currentPv.push_back(move);
                currentPv.insert(currentPv.end(), childPv.begin(), childPv.end());
            }
            beta = std::min(beta, bestScore);
        }

        // ��������� Alpha-Beta
        if (beta <= alpha) {
            break;
        }
    }

    // ��������� ��������� � ������� ������������
    TranspositionEntry entry;
    entry.depth = depth;
    entry.score = bestScore;
    entry.bestMove = bestMove;

    // ���������� ��� ����
    if (bestScore <= originalAlpha) {
        entry.flag = ALPHA;
    }
    else if (bestScore >= beta) {
        entry.flag = BETA;
    }
    else {
        entry.flag = EXACT;
    }

    transpositionTable[positionHash] = entry;

    // ��������� principal variation ��� ������������� ����
    pv = currentPv;

    return bestScore;
}

bool MateAnalyzer::isMateScore(int score) const {
    return abs(score) > MATE_VALUE - 100; // ��������, ��� ������ ������ � MATE_VALUE
}

std::string MateAnalyzer::calculatePositionHash(const Board& board) const {
    // ������� ���: ������������� ����� � ���� ������
    std::stringstream ss;

    // ��������� ������� ���� �����
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Position pos(i, j);
            ChessPiece* piece = board.getPieceAt(pos);
            if (piece) {
                ss << piece->getSymbol() << i << j;
            }
        }
    }

    return ss.str();
}

std::vector<Move> MateAnalyzer::generateOrderedMoves(const Board& board, Color color) const {
    // �������� ��� ��������� ����
    std::vector<Move> allMoves;
    auto pieces = board.getPiecesByColor(color);

    // ����������� ��� �������� � insert � ������
    for (auto piece : pieces) {
        std::vector<Move> pieceMoves = piece->getPossibleMoves(board);
        allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
    }

    // ��� ������� ���� ��������������:
    // 1. ���� (����, ������� ������ ������ ���������� ��� ���)
    // 2. ������ (����, ������� �������� � ������ ������ ����������)
    // 3. ��� ��������� ����

    auto isCheck = [&](const Move& move) {
        Board tmpBoard = board;
        tmpBoard.makeMove(move);
        return tmpBoard.isKingInCheck(color == Color::WHITE ? Color::BLACK : Color::WHITE);
        };

    auto isCapture = [&](const Move& move) {
        return board.getPieceAt(move.to) != nullptr;
        };

    // ��������� ���� �� ����������
    std::stable_sort(allMoves.begin(), allMoves.end(), [&](const Move& a, const Move& b) {
        bool aIsCheck = isCheck(a);
        bool bIsCheck = isCheck(b);

        if (aIsCheck && !bIsCheck) return true;
        if (!aIsCheck && bIsCheck) return false;

        bool aIsCapture = isCapture(a);
        bool bIsCapture = isCapture(b);

        if (aIsCapture && !bIsCapture) return true;
        if (!aIsCapture && bIsCapture) return false;

        return false;
        });

    return allMoves;
}
