// MateAnalyzer.cpp
#include "MateAnalyzer.h"
#include "ChessPiece.h" // Необходимо для исправления ошибки C2027
#include <algorithm>
#include <sstream>

MateAnalyzer::MateAnalyzer() {
}

bool MateAnalyzer::canMateInNMoves(const Board& board, Color attackingColor, int maxMoves) const {
    // Создаем копию доски, так как мы будем её модифицировать
    Board boardCopy = board;

    // Для проверки мата в N ходов нужно искать на глубину 2*N (полуходов)
    int depth = 2 * maxMoves;

    // Инициализируем переменные для поиска
    std::vector<Move> principalVariation;
    int alpha = -MATE_VALUE - 1;
    int beta = MATE_VALUE + 1;

    // Запускаем поиск
    int score = alphaBeta(boardCopy, depth, alpha, beta,
        attackingColor == Color::WHITE, principalVariation, true);

    // Если найден мат, сохраняем последовательность ходов
    if (isMateScore(score)) {
        currentMatingSequence = principalVariation;
        return true;
    }

    return false;
}

std::vector<Move> MateAnalyzer::getMatingSequence(const Board& board, Color attackingColor, int maxMoves) const {
    // Если мы еще не проанализировали позицию, сделаем это сейчас
    if (currentMatingSequence.empty()) {
        canMateInNMoves(board, attackingColor, maxMoves);
    }

    return currentMatingSequence;
}

bool MateAnalyzer::canMateInNMovesIterativeDeepening(const Board& board, Color attackingColor, int maxMoves) const {
    // Очищаем таблицу транспозиций для нового анализа
    transpositionTable.clear();

    // Начинаем с глубины 1 и постепенно увеличиваем до максимальной
    for (int currentDepth = 1; currentDepth <= maxMoves; currentDepth++) {
        if (canMateInNMoves(board, attackingColor, currentDepth)) {
            return true;
        }
    }

    return false;
}

int MateAnalyzer::alphaBeta(Board& board, int depth, int alpha, int beta, bool maximizingPlayer,
    std::vector<Move>& pv, bool rootNode) const {
    // Базовый случай: достигнута максимальная глубина или терминальная позиция
    if (depth == 0) {
        // Для анализа мата не используем стандартную оценку позиции,
        // а только проверяем, является ли позиция матом или нет
        Color defendingColor = maximizingPlayer ? Color::BLACK : Color::WHITE;
        Color attackingColor = maximizingPlayer ? Color::WHITE : Color::BLACK;

        if (board.isKingInCheckmate(defendingColor)) {
            return maximizingPlayer ? MATE_VALUE + depth : -MATE_VALUE - depth;
        }

        // Оценка с учетом расстояния до мата
        if (board.isKingInCheck(defendingColor)) {
            int score = 500; // Базовая оценка для шаха

            // Добавляем оценку за ограниченную подвижность короля
            int kingMoves = board.getKingMobilityCount(defendingColor);
            score += (8 - kingMoves) * 50; // Чем меньше ходов у короля, тем лучше

            // Учитываем количество атакующих фигур вблизи короля
            Position kingPos = board.findKing(defendingColor);
            int attackersNearKing = board.countAttackersNearKing(attackingColor, kingPos);
            score += attackersNearKing * 100;

            return maximizingPlayer ? score : -score;
        }

        // В случае пата или обычной позиции возвращаем 0 (не мат)
        return 0;
    }

    // Формируем хеш позиции
    std::string positionHash = calculatePositionHash(board);

    // Проверяем, есть ли позиция в таблице транспозиций
    if (transpositionTable.find(positionHash) != transpositionTable.end()) {
        TranspositionEntry entry = transpositionTable[positionHash];

        // Используем информацию из таблицы, если глубина поиска была достаточной
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

    // PV-поиск: сохраняем первоначальное значение альфа для определения,
    // был ли найден точный результат или только граница
    int originalAlpha = alpha;

    // Текущая PV для этого узла
    std::vector<Move> currentPv;

    // Генерируем ходы в оптимальном порядке
    Color currentColor = maximizingPlayer ? Color::WHITE : Color::BLACK;
    std::vector<Move> moves = generateOrderedMoves(board, currentColor);

    // Если нет ходов, проверяем на мат или пат
    if (moves.empty()) {
        if (board.isKingInCheck(currentColor)) {
            // Мат: возвращаем -MATE_VALUE с поправкой на глубину
            return maximizingPlayer ? -MATE_VALUE - depth : MATE_VALUE + depth;
        }
        else {
            // Пат: возвращаем 0
            return 0;
        }
    }

    Move bestMove;
    int bestScore = maximizingPlayer ? -MATE_VALUE - 1 : MATE_VALUE + 1;

    // Исследуем все ходы
    for (const Move& move : moves) {
        // Сохраняем текущее состояние доски
        Board originalBoard = board;

        // Делаем ход
        if (!board.makeMove(move)) {
            continue; // Ход недопустим
        }

        // Рекурсивно вычисляем оценку
        std::vector<Move> childPv;
        int score = alphaBeta(board, depth - 1, alpha, beta, !maximizingPlayer, childPv, false);

        // Восстанавливаем состояние доски
        board = originalBoard;

        // Обновляем лучший результат
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

        // Отсечение Alpha-Beta
        if (beta <= alpha) {
            break;
        }
    }

    // Сохраняем результат в таблице транспозиций
    TranspositionEntry entry;
    entry.depth = depth;
    entry.score = bestScore;
    entry.bestMove = bestMove;

    // Определяем тип узла
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

    // Обновляем principal variation для родительского узла
    pv = currentPv;

    return bestScore;
}

bool MateAnalyzer::isMateScore(int score) const {
    return abs(score) > MATE_VALUE - 100; // Проверка, что оценка близка к MATE_VALUE
}

std::string MateAnalyzer::calculatePositionHash(const Board& board) const {
    // Простой хеш: представление доски в виде строки
    std::stringstream ss;

    // Добавляем позиции всех фигур
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
    // Получаем все возможные ходы
    std::vector<Move> allMoves;
    auto pieces = board.getPiecesByColor(color);

    // Исправление для проблемы с insert в вектор
    for (auto piece : pieces) {
        std::vector<Move> pieceMoves = piece->getPossibleMoves(board);
        allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
    }

    // Для анализа мата приоритизируем:
    // 1. Шахи (ходы, которые ставят короля противника под шах)
    // 2. Взятия (ходы, которые приводят к взятию фигуры противника)
    // 3. Все остальные ходы

    auto isCheck = [&](const Move& move) {
        Board tmpBoard = board;
        tmpBoard.makeMove(move);
        return tmpBoard.isKingInCheck(color == Color::WHITE ? Color::BLACK : Color::WHITE);
        };

    auto isCapture = [&](const Move& move) {
        return board.getPieceAt(move.to) != nullptr;
        };

    // Сортируем ходы по приоритету
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
