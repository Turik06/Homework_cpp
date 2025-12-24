// MateAnalyzer.h
#ifndef MATE_ANALYZER_H
#define MATE_ANALYZER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "Board.h"
#include "ChessTypes.h"

// Структура для хранения информации в таблице транспозиций
struct TranspositionEntry {
    int depth;      // Глубина поиска
    int score;      // Оценка позиции
    int flag;       // Тип оценки: точная, верхняя граница, нижняя граница
    Move bestMove;  // Лучший ход
};

// Флаги для таблицы транспозиций
enum NodeType {
    EXACT = 0,      // Точная оценка
    ALPHA = 1,      // Верхняя граница
    BETA = 2        // Нижняя граница
};

class MateAnalyzer {
public:
    MateAnalyzer();

    // Проверяет возможность постановки мата в заданное количество ходов
    bool canMateInNMoves(const Board& board, Color attackingColor, int maxMoves) const;

    // Возвращает матующую последовательность ходов
    std::vector<Move> getMatingSequence(const Board& board, Color attackingColor, int maxMoves) const;

    // Проверяет возможность постановки мата с итеративным углублением
    bool canMateInNMovesIterativeDeepening(const Board& board, Color attackingColor, int maxMoves) const;

private:
    // Реализация Minimax с Alpha-Beta отсечением
    int alphaBeta(Board& board, int depth, int alpha, int beta, bool maximizingPlayer,
        std::vector<Move>& pv, bool rootNode) const;

    // Проверяет, является ли оценка матовой
    bool isMateScore(int score) const;

    // Вычисляет хеш позиции для таблицы транспозиций
    std::string calculatePositionHash(const Board& board) const;

    // Таблица транспозиций - mutable для модификации в const методах
    mutable std::unordered_map<std::string, TranspositionEntry> transpositionTable;

    // Генерирует ходы для анализа в приоритетном порядке
    std::vector<Move> generateOrderedMoves(const Board& board, Color color) const;

    // Максимальная глубина поиска
    static const int MAX_DEPTH = 8; // 4 хода = 8 полуходов

    // Оценка для мата
    static const int MATE_VALUE = 10000;

    // Текущая последовательность ходов для мата - mutable для модификации в const методах
    mutable std::vector<Move> currentMatingSequence;
};

#endif // MATE_ANALYZER_H
