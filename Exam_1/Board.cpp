// Board.cpp
#include "Board.h"
#include "ChessPiece.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Board::Board() {
    clear();
}

Board::~Board() {
    clear();
}

// Копирующий конструктор
Board::Board(const Board& other) {
    clear(); // Очищаем нашу доску

    // Копируем все фигуры с другой доски
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (other.board[i][j]) {
                // Используем clone() для создания глубокой копии фигуры
                board[i][j] = other.board[i][j]->clone();
            }
        }
    }
}

// Оператор присваивания
Board& Board::operator=(const Board& other) {
    if (this != &other) { // Проверка на самоприсваивание
        clear(); // Очищаем текущую доску

        // Копируем все фигуры с другой доски
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (other.board[i][j]) {
                    // Используем clone() для создания глубокой копии фигуры
                    board[i][j] = other.board[i][j]->clone();
                }
            }
        }
    }
    return *this;
}

void Board::clear() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }
}

void Board::addPiece(std::unique_ptr<ChessPiece> piece) {
    Position pos = piece->getPosition();
    if (pos.isValid()) {
        board[pos.x][pos.y] = std::move(piece);
    }
}

void Board::removePiece(const Position& position) {
    if (position.isValid()) {
        board[position.x][position.y] = nullptr;
    }
}

ChessPiece* Board::getPieceAt(const Position& position) const {
    if (position.isValid()) {
        return board[position.x][position.y].get();
    }
    return nullptr;
}

bool Board::makeMove(const Move& move) {
    ChessPiece* piece = getPieceAt(move.from);
    if (!piece || !piece->isValidMove(*this, move)) {
        return false;
    }

    // Создаем временное хранилище для перемещаемой фигуры
    std::unique_ptr<ChessPiece> movedPiece = std::move(board[move.from.x][move.from.y]);

    // Удаляем фигуру с целевой позиции, если она там есть
    board[move.to.x][move.to.y] = nullptr;

    // Обновляем позицию передвигаемой фигуры
    movedPiece->setPosition(move.to);

    // Помещаем фигуру на новую позицию
    board[move.to.x][move.to.y] = std::move(movedPiece);

    return true;
}

std::vector<ChessPiece*> Board::getPiecesByColor(Color color) const {
    std::vector<ChessPiece*> pieces;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board[i][j].get();
            if (piece && piece->getColor() == color) {
                pieces.push_back(piece);
            }
        }
    }

    return pieces;
}

Position Board::findKing(Color color) const {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board[i][j].get();
            if (piece && piece->getColor() == color &&
                piece->getType() == PieceType::KING) {
                return Position(i, j);
            }
        }
    }

    // Если король не найден, возвращаем невалидную позицию
    return Position(-1, -1);
}

bool Board::isPositionAttacked(const Position& position, Color attackingColor) const {
    // Проверяем все фигуры атакующего цвета
    auto pieces = getPiecesByColor(attackingColor);

    for (auto piece : pieces) {
        // Для каждой фигуры проверяем, может ли она атаковать указанную позицию
        Move move(piece->getPosition(), position);
        if (piece->isValidMove(*this, move)) {
            return true;
        }
    }

    return false;
}

bool Board::isKingInCheck(Color color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) {
        return false; // Короля нет на доске
    }

    // Проверяем, атакуется ли король фигурами противоположного цвета
    return isPositionAttacked(kingPos, color == Color::WHITE ? Color::BLACK : Color::WHITE);
}

bool Board::isKingInCheckmate(Color color) const {
    // Если король не под шахом, то и тем более не под матом
    if (!isKingInCheck(color)) {
        return false;
    }

    // Получаем все фигуры указанного цвета
    auto pieces = getPiecesByColor(color);

    // Проверяем каждую фигуру и каждый возможный ход
    for (auto piece : pieces) {
        auto moves = piece->getPossibleMoves(*this);

        for (const auto& move : moves) {
            // Создаем копию доски для симуляции хода
            Board boardCopy = *this;

            // Выполняем ход на копии
            boardCopy.makeMove(move);

            // Проверяем, остался ли король под шахом после хода
            if (!boardCopy.isKingInCheck(color)) {
                return false; // Нашли ход, который спасает от шаха
            }
        }
    }

    // Если ни один ход не спасает от шаха, то это мат
    return true;
}

bool Board::canCheckmateInOneMove(Color color) const {
    // Получаем все фигуры указанного цвета
    auto pieces = getPiecesByColor(color);

    // Проверяем каждую фигуру и каждый возможный ход
    for (auto piece : pieces) {
        auto moves = piece->getPossibleMoves(*this);

        for (const auto& move : moves) {
            // Создаем копию доски для симуляции хода
            Board boardCopy = *this;

            // Выполняем ход на копии
            boardCopy.makeMove(move);

            // Проверяем, привел ли ход к мату
            if (boardCopy.isKingInCheckmate(color == Color::WHITE ? Color::BLACK : Color::WHITE)) {
                return true;
            }
        }
    }

    return false;
}

bool Board::canCheckmateInTwoMoves(Color color) const {
    // Сначала проверяем мат в 1 ход
    if (canCheckmateInOneMove(color)) {
        return true;
    }

    // Получаем все фигуры указанного цвета
    auto pieces = getPiecesByColor(color);

    // Проверяем каждую фигуру и каждый возможный ход
    for (auto piece : pieces) {
        auto moves = piece->getPossibleMoves(*this);

        for (const auto& move : moves) {
            // Создаем копию доски для симуляции первого хода
            Board boardCopy = *this;

            // Выполняем первый ход на копии
            boardCopy.makeMove(move);

            // Проверяем, может ли противник сделать ход
            Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
            auto opponentPieces = boardCopy.getPiecesByColor(opponentColor);

            bool opponentCanMove = false;
            bool opponentCanAvoidMate = false;

            for (auto opponentPiece : opponentPieces) {
                auto opponentMoves = opponentPiece->getPossibleMoves(boardCopy);

                // Если у противника есть ходы
                if (!opponentMoves.empty()) {
                    opponentCanMove = true;

                    for (const auto& opponentMove : opponentMoves) {
                        // Создаем ещё одну копию для симуляции хода противника
                        Board boardCopy2 = boardCopy;
                        boardCopy2.makeMove(opponentMove);

                        // Проверяем, можем ли мы поставить мат после ответа противника
                        if (!boardCopy2.canCheckmateInOneMove(color)) {
                            opponentCanAvoidMate = true;
                            break;
                        }
                    }

                    if (opponentCanAvoidMate) {
                        break;
                    }
                }
            }

            // Если у противника нет ходов или все его ходы ведут к мату в следующем ходу
            if (!opponentCanMove || !opponentCanAvoidMate) {
                return true;
            }
        }
    }

    return false;
}

int Board::getPieceCount() const {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) {
                count++;
            }
        }
    }
    return count;
}

int Board::getKingMobilityCount(Color color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) {
        return 0;
    }

    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            Position newPos(kingPos.x + dx, kingPos.y + dy);
            if (newPos.isValid()) {
                ChessPiece* piece = getPieceAt(newPos);
                if (!piece || piece->getColor() != color) {
                    // Проверяем, не находится ли позиция под атакой
                    if (!isPositionAttacked(newPos, color == Color::WHITE ? Color::BLACK : Color::WHITE)) {
                        count++;
                    }
                }
            }
        }
    }

    return count;
}

int Board::countAttackersNearKing(Color attackingColor, const Position& kingPos) const {
    int count = 0;
    auto attackers = getPiecesByColor(attackingColor);

    for (auto attacker : attackers) {
        Position pos = attacker->getPosition();
        int dx = std::abs(pos.x - kingPos.x);
        int dy = std::abs(pos.y - kingPos.y);

        // Считаем фигуры в радиусе 2 клеток от короля
        if (dx <= 2 && dy <= 2) {
            count++;
        }
    }

    return count;
}

void Board::print() const {
    std::cout << "  a b c d e f g h\n";
    std::cout << " +-----------------+\n";

    for (int j = 7; j >= 0; j--) {
        std::cout << j + 1 << "| ";

        for (int i = 0; i < 8; i++) {
            ChessPiece* piece = board[i][j].get();
            if (piece) {
                std::cout << piece->getSymbol() << " ";
            }
            else {
                std::cout << ". ";
            }
        }

        std::cout << "|\n";
    }

    std::cout << " +-----------------+\n";
}
