#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <set>
#include <map>
#include <limits>
#include "Board.h"
#include "ChessTypes.h"

class ChessPiece;

class Game {
public:
    Game(); // Конструктор игры

    bool loadFromFile(const std::string& filename); // Загрузка позиции из файла
    void printBoard() const; // Печать текущей доски
    bool isCurrentPositionMate() const; // Проверка: мат ли на текущей позиции

    std::map<int, std::set<std::string>> findAllMateSequences(int maxDepthFullMoves = 3) const; // Поиск всех матов в N ходов
private:
    Board board; // Шахматная доска и фигуры

    std::unique_ptr<ChessPiece> createPiece(char symbol, const Position& pos) const; // Создание фигуры по символу
    std::vector<Move> generateOrderedMoves(const Board& currentBoard, Color color) const; // Сортированные ходы (по эвристике)
    int evaluateMove(const Board& currentBoard, const Move& move, Color color) const; // Оценка отдельного хода
    int getPieceValue(PieceType type) const; // "Цена" фигуры (для оценки позиции)
    std::vector<Move> getAllLegalMoves(const Board& currentBoard, Color color) const; // Все допустимые ходы для цвета
    std::string movesToString(const std::vector<Move>& moves) const; // Преобразование последовательности ходов в строку

    int evaluateBoard(const Board& currentBoard, Color playerToMove) const; // Оценка текущей позиции
    int alphaBetaSearch(Board currentBoard, int depthInPlies, int alpha, int beta, Color playerToMove,
        std::vector<Move>& bestMoveSequenceForThisNode) const; // Поиск по дереву с отсечением

    std::set<std::string> findForcedMate_AlphaBeta(int numFullMoves) const; // Поиск форсированных матов в N ходов

    bool compareMovesByHeuristics(const Board& currentBoard, Color color, const Move& a, const Move& b) const; // Сравнение двух ходов по эвристике

    static const int MATE_SCORE = 1000000; // Оценка мата
    static const int INFINITY_SCORE = MATE_SCORE + 1000; // "Бесконечная" оценка
};

#endif
