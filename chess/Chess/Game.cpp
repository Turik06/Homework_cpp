#include "Game.h"
#include "ChessPieces.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <fstream>

// --- Таблицы позиционной оценки (Piece-Square Tables) ---
// Оценки даются для БЕЛЫХ. Для черных индексы инвертируются по вертикали.
// Индекс [y][x], где y - ряд (0-7), x - вертикаль (0-7).

// Пешки: поощряем продвижение к центру и превращению
const int pawnTable[8][8] = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    { 5,  5, 10, 25, 25, 10,  5,  5},
    { 0,  0,  0, 20, 20,  0,  0,  0},
    { 5, -5,-10,  0,  0,-10, -5,  5},
    { 5, 10, 10,-20,-20, 10, 10,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0}
};

// Кони: ОЧЕНЬ сильный штраф за стояние на задней линии (-50 -> -80)
const int knightTable[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-80,-50,-40,-40,-40,-40,-50,-80} 
};

// Слоны: тоже штрафуем за пассивность
const int bishopTable[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-50,-20,-20,-20,-20,-20,-20,-50} 
};

// Ладьи: бонус за 7-ю горизонталь и вертикали
const int rookTable[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0},
    {  5, 10, 10, 10, 10, 10, 10,  5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    {  0,  0,  0,  5,  5,  0,  0,  0}
};

// Королева: штраф за ранний вывод
const int queenTable[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    { -5,  0,  5,  5,  5,  5,  0, -5},
    {  0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20}
};

// Король (Миттельшпиль): безопасность в углу
const int kingTable[8][8] = {
    { 20, 30, 10,  0,  0, 10, 30, 20},
    { 20, 20,  0,  0,  0,  0, 20, 20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30}
};

// Вспомогательная функция для получения бонуса позиции
int getPositionBonus(const ChessPiece* piece, const Position& pos) {
    int x = pos.x;
    int y = pos.y;

    // Если фигура черная, зеркалим таблицу по вертикали (y)
    // Предполагаем, что y=0 это низ (белые), y=7 это верх (черные)
    if (piece->getColor() == Color::BLACK) {
        y = 7 - y;
    }

    switch (piece->getType()) {
        case PieceType::PAWN:   return pawnTable[y][x];
        case PieceType::KNIGHT: return knightTable[y][x];
        case PieceType::BISHOP: return bishopTable[y][x];
        case PieceType::ROOK:   return rookTable[y][x];
        case PieceType::QUEEN:  return queenTable[y][x];
        case PieceType::KING:   return kingTable[y][x];
        default: return 0;
    }
}

// Конструктор игры
Game::Game() {
    setupNewGame();
}

void Game::loadConfig(const std::string& filename) {
    config.load(filename);
}

// Настройка новой партии
void Game::setupNewGame() {
    board.clear(); // Очищаем доску
    turn = Color::WHITE; // Начинают белые
    
    // Расстановка белых фигур (первая горизонталь)
    board.addPiece(std::make_unique<Rook>(Color::WHITE, Position("a1")));
    board.addPiece(std::make_unique<Knight>(Color::WHITE, Position("b1")));
    board.addPiece(std::make_unique<Bishop>(Color::WHITE, Position("c1")));
    board.addPiece(std::make_unique<Queen>(Color::WHITE, Position("d1")));
    board.addPiece(std::make_unique<King>(Color::WHITE, Position("e1")));
    board.addPiece(std::make_unique<Bishop>(Color::WHITE, Position("f1")));
    board.addPiece(std::make_unique<Knight>(Color::WHITE, Position("g1")));
    board.addPiece(std::make_unique<Rook>(Color::WHITE, Position("h1")));
    
    // Расстановка белых пешек (вторая горизонталь)
    for (int i = 0; i < 8; ++i) {
        board.addPiece(std::make_unique<Pawn>(Color::WHITE, Position(i, 1)));
    }
    
    // Расстановка черных фигур (восьмая горизонталь)
    board.addPiece(std::make_unique<Rook>(Color::BLACK, Position("a8")));
    board.addPiece(std::make_unique<Knight>(Color::BLACK, Position("b8")));
    board.addPiece(std::make_unique<Bishop>(Color::BLACK, Position("c8")));
    board.addPiece(std::make_unique<Queen>(Color::BLACK, Position("d8")));
    board.addPiece(std::make_unique<King>(Color::BLACK, Position("e8")));
    board.addPiece(std::make_unique<Bishop>(Color::BLACK, Position("f8")));
    board.addPiece(std::make_unique<Knight>(Color::BLACK, Position("g8")));
    board.addPiece(std::make_unique<Rook>(Color::BLACK, Position("h8")));
    
    // Расстановка черных пешек (седьмая горизонталь)
    for (int i = 0; i < 8; ++i) {
        board.addPiece(std::make_unique<Pawn>(Color::BLACK, Position(i, 6)));
    }
}

// Выполнение хода
bool Game::makeMove(const Move& move) {
    // Получаем список всех легальных ходов для текущего игрока
    auto legalMoves = getAllLegalMoves(board, turn);
    
    // Проверяем, является ли данный ход легальным
    bool isLegal = false;
    for (const auto& legalMove : legalMoves) {
        if (legalMove.from == move.from && legalMove.to == move.to && 
            legalMove.promotionType == move.promotionType) {
            isLegal = true;
            break;
        }
    }

    // Если ход нелегальный, отклоняем его
    if (!isLegal) {
        return false;
    }

    // Получаем фигуру на начальной позиции
    ChessPiece* piece = board.getPieceAt(move.from);
    if (!piece) return false; // Если фигуры нет, ход невозможен

    // Запоминаем тип и цвет фигуры для обработки превращения пешки
    PieceType pieceType = piece->getType();
    Color pieceColor = piece->getColor();

    // Выполняем ход на доске
    board.makeMove(move);

    // Обработка превращения пешки
    if (pieceType == PieceType::PAWN && move.promotionType != PieceType::KING) {
        board.removePiece(move.to); // Удаляем пешку
        std::unique_ptr<ChessPiece> newPiece;
        
        // Создаем новую фигуру в зависимости от типа превращения
        switch (move.promotionType) {
            case PieceType::QUEEN:  newPiece = std::make_unique<Queen>(pieceColor, move.to); break;
            case PieceType::ROOK:   newPiece = std::make_unique<Rook>(pieceColor, move.to); break;
            case PieceType::BISHOP: newPiece = std::make_unique<Bishop>(pieceColor, move.to); break;
            case PieceType::KNIGHT: newPiece = std::make_unique<Knight>(pieceColor, move.to); break;
            default: break;
        }
        
        // Добавляем новую фигуру на доску
        if (newPiece) {
            newPiece->setHasMoved(true); // Помечаем, что фигура уже двигалась
            board.addPiece(std::move(newPiece));
        }
    }

    // Передаем ход другому игроку
    turn = (turn == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return true;
}

// Вывод доски на экран
void Game::printBoard() const {
    board.print();
}

// Проверка окончания игры
bool Game::isGameOver(std::string& reason) {
    // Получаем все легальные ходы для текущего игрока
    auto legalMoves = getAllLegalMoves(board, turn);

    // Если нет легальных ходов
    if (legalMoves.empty()) {
        // Проверяем, находится ли король под шахом
        if (board.isKingInCheck(turn)) {
            // Мат: победа противника
            reason = (turn == Color::WHITE) ? "Checkmate! Black wins." : "Checkmate! White wins.";
        } else {
            // Пат: ничья
            reason = "Stalemate! It's a draw.";
        }
        return true;
    }
    return false;
}

// Возвращает ценность фигуры в пешках (используется в evaluateBoard)
int Game::getPieceValue(PieceType type) const {
    switch (type) {
        case PieceType::PAWN:   return config.pawnValue;
        case PieceType::KNIGHT: return config.knightValue;
        case PieceType::BISHOP: return config.bishopValue;
        case PieceType::ROOK:   return config.rookValue;
        case PieceType::QUEEN:  return config.queenValue;
        case PieceType::KING:   return config.kingValue;
        default: return 0;
    }
}

// Оценочная функция позиции (статическая оценка)
int Game::evaluateBoard(const Board& currentBoard) const {
    int totalScore = 0;
    
    // 1. Материал и Позиция (PST)
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            ChessPiece* piece = currentBoard.getPieceAt(Position(i, j));
            if (piece) {
                int pieceValue = getPieceValue(piece->getType());
                int positionBonus = getPositionBonus(piece, Position(i, j));
                
                // Бонус за мобильность (количество доступных ходов)
                // Это "тяжелая" операция, но на глубине 3 она допустима
                // Мы даем по 5 очков за каждый возможный ход
                int mobilityBonus = 0;
                
                // Считаем мобильность только для легких и тяжелых фигур (не пешек и королей)
                if (piece->getType() != PieceType::PAWN && piece->getType() != PieceType::KING) {
                     // Получаем количество ходов (просто размер вектора)
                     // Внимание: это замедлит перебор, но улучшит качество
                     mobilityBonus = piece->getPossibleMoves(currentBoard).size() * 5;
                }

                int finalValue = pieceValue + positionBonus + mobilityBonus;

                if (piece->getColor() == Color::WHITE) {
                    totalScore += finalValue;
                } else {
                    totalScore -= finalValue;
                }
            }
        }
    }
    return totalScore; 
}

// Алгоритм альфа-бета отсечения для поиска лучшего хода
int Game::alphaBeta(Board currentBoard, int depth, int alpha, int beta, bool maximizingPlayer, std::ofstream* logger, std::string branchId) {
    // 1. Логируем вход в ветку с текущими параметрами
    if (logger) {
        *logger << "Branch[" << branchId << "] Depth: " << depth 
                << " Alpha: " << alpha << " Beta: " << beta 
                << " Max: " << (maximizingPlayer ? "true" : "false") << "\n";
    }

    // 2. Базовый случай: достигнута максимальная глубина поиска
    if (depth == 0) {
        int eval = evaluateBoard(currentBoard);
        // Логируем результат оценочной функции на листе дерева
        if (logger) {
            *logger << "Branch[" << branchId << "] LEAF. Evaluation: " << eval << "\n";
        }
        return eval;
    }

    // Определяем цвет текущего игрока
    Color playerColor = maximizingPlayer ? Color::WHITE : Color::BLACK;
    auto legalMoves = getAllLegalMoves(currentBoard, playerColor);

    // 3. Если нет легальных ходов (мат или пат)
    if (legalMoves.empty()) {
        int eval = 0;
        if (currentBoard.isKingInCheck(playerColor)) {
            // Мат: возвращаем очень плохую оценку для текущего игрока
            eval = maximizingPlayer ? -MATE_SCORE : MATE_SCORE;
        }
        // Иначе пат (eval = 0)
        
        if (logger) {
            *logger << "Branch[" << branchId << "] GAME OVER. Eval: " << eval << "\n";
        }
        return eval;
    }

    int moveCounter = 1; // Счетчик для генерации ID под-веток (1.1, 1.2 и т.д.)

    // Максимизирующий игрок (белые)
    if (maximizingPlayer) {
        int maxEval = -INFINITY_SCORE;
        
        for (const auto& move : legalMoves) {
            Board nextBoard = currentBoard;
            nextBoard.makeMove(move);
            
            // Формируем ID следующей ветки
            std::string nextBranchId = branchId + "." + std::to_string(moveCounter++);

            // Рекурсивный вызов
            int eval = alphaBeta(nextBoard, depth - 1, alpha, beta, false, logger, nextBranchId);
            
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            
            // Альфа-бета отсечение
            if (beta <= alpha) {
                if (logger) {
                    *logger << "Branch[" << branchId << "] PRUNED at depth " << depth 
                            << " (Beta " << beta << " <= Alpha " << alpha << ")\n";
                }
                break; // Прекращаем перебор
            }
        }
        return maxEval;
    } 
    // Минимизирующий игрок (черные)
    else {
        int minEval = INFINITY_SCORE;
        
        for (const auto& move : legalMoves) {
            Board nextBoard = currentBoard;
            nextBoard.makeMove(move);
            
            // Формируем ID следующей ветки
            std::string nextBranchId = branchId + "." + std::to_string(moveCounter++);

            // Рекурсивный вызов
            int eval = alphaBeta(nextBoard, depth - 1, alpha, beta, true, logger, nextBranchId);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            // Альфа-бета отсечение
            if (beta <= alpha) {
                if (logger) {
                    *logger << "Branch[" << branchId << "] PRUNED at depth " << depth 
                            << " (Beta " << beta << " <= Alpha " << alpha << ")\n";
                }
                break; // Прекращаем перебор
            }
        }
        return minEval;
    }
}

// Поиск лучшего хода на заданной глубине
Move Game::findBestMove(int depth) {
    int searchDepth = (depth != -1) ? depth : config.searchDepth;
    
    std::ofstream logger;
    std::ofstream* loggerPtr = nullptr;
    
    // Настройка логгера
    if (config.logAlphaBeta) {
        // Используем trunc, чтобы очищать файл при каждом новом поиске, или app для добавления
        logger.open(config.logFilePath, std::ios::app); 
        if (logger.is_open()) {
            logger << "\n--- New Search (Depth " << searchDepth << ") ---\n";
            loggerPtr = &logger;
        }
    }

    Move bestMove;
    // Инициализируем худшим возможным значением для текущего игрока
    int bestValue = (turn == Color::WHITE) ? -INFINITY_SCORE : INFINITY_SCORE;
    
    auto legalMoves = getAllLegalMoves(board, turn);
    
    int moveIndex = 1; // Счетчик для корневых веток (1, 2, 3...)

    // Перебираем все легальные ходы
    for (const auto& move : legalMoves) {
        Board nextBoard = board;
        nextBoard.makeMove(move);

        // Формируем ID корневой ветки
        std::string currentBranchId = std::to_string(moveIndex++);

        // Вычисляем оценку позиции после хода
        // Обратите внимание: передаем currentBranchId
        int boardValue = alphaBeta(nextBoard, searchDepth - 1, -INFINITY_SCORE, INFINITY_SCORE, turn == Color::BLACK, loggerPtr, currentBranchId);

        // Обновляем лучший ход
        if (turn == Color::WHITE) {
            // Белые ищут максимум
            if (boardValue > bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        } else {
            // Черные ищут минимум
            if (boardValue < bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        }
    }
    
    if (logger.is_open()) {
        logger << "Best Move: " << bestMove.toString() << " Value: " << bestValue << "\n";
        logger.close();
    }

    return bestMove;

}

// Генерация всех легальных ходов для данного цвета
std::vector<Move> Game::getAllLegalMoves(const Board& currentBoard, Color color) const {
    std::vector<Move> legalMoves;
    
    // Получаем все фигуры данного цвета
    auto pieces = currentBoard.getPiecesByColor(color);

    // Для каждой фигуры
    for (ChessPiece* piece : pieces) {
        // Получаем все возможные ходы
        auto possibleMoves = piece->getPossibleMoves(currentBoard);
        
        for (const Move& move : possibleMoves) {
            // Проверяем, не оставляет ли ход короля под шахом
            Board testBoard = currentBoard;
            testBoard.makeMove(move);
            
            // Если после хода король не под шахом, ход легален
            if (!testBoard.isKingInCheck(color)) {
                legalMoves.push_back(move);
            }
        }
    }

    // Сортируем ходы для оптимизации альфа-бета отсечения
    // 1. Взятия с большей ценностью рассматриваются первыми
    // 2. Если взятия нет, рассматриваем ходы, улучшающие позицию
    std::sort(legalMoves.begin(), legalMoves.end(), [&](const Move& a, const Move& b) {
        ChessPiece* capturedA = currentBoard.getPieceAt(a.to);
        ChessPiece* capturedB = currentBoard.getPieceAt(b.to);
        
        // Вычисляем ценность взятых фигур
        int valueA = capturedA ? getPieceValue(capturedA->getType()) : 0;
        int valueB = capturedB ? getPieceValue(capturedB->getType()) : 0;
        
        // Если одно взятие ценнее другого, оно идет первым
        if (valueA != valueB) {
            return valueA > valueB; 
        }

        // Если взятия равны (или их нет), сравниваем позиционную выгоду
        ChessPiece* pieceA = currentBoard.getPieceAt(a.from);
        ChessPiece* pieceB = currentBoard.getPieceAt(b.from);

        if (pieceA && pieceB) {
            // Вычисляем разницу: (бонус на новой клетке) - (бонус на старой клетке)
            int bonusA = getPositionBonus(pieceA, a.to) - getPositionBonus(pieceA, a.from);
            int bonusB = getPositionBonus(pieceB, b.to) - getPositionBonus(pieceB, b.from);
            return bonusA > bonusB;
        }

        return false;
    });

    return legalMoves;
}