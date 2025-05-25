#include "Board.h"
#include "ChessPieces.h"
#include <iostream>
#include <algorithm>

Board::Board() {
    clear();
}

Board::~Board() {
    clear();
}

Board::Board(const Board& other) {
    clear();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (other.board[i][j]) {
                board[i][j] = other.board[i][j]->clone();
            }
        }
    }
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        clear();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (other.board[i][j]) {
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

    // Сохраняем взятую фигуру
    std::unique_ptr<ChessPiece> capturedPiece = std::move(board[move.to.x][move.to.y]);

    // Перемещаем фигуру
    std::unique_ptr<ChessPiece> movedPiece = std::move(board[move.from.x][move.from.y]);
    movedPiece->setPosition(move.to);
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
    return Position(-1, -1);
}

bool Board::isPositionAttacked(const Position& position, Color attackingColor) const {
    auto pieces = getPiecesByColor(attackingColor);
    for (auto piece : pieces) {
        Move move(piece->getPosition(), position);
        if (piece->isValidMove(*this, move)) {
            return true;
        }
    }
    return false;
}

bool Board::isKingInCheck(Color color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) return false;
    return isPositionAttacked(kingPos, color == Color::WHITE ? Color::BLACK : Color::WHITE);
}

bool Board::isKingInCheckmate(Color color) const {
    
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) {
      
        return true;
    }

    if (!isKingInCheck(color)) { 
        return false;
    }

    auto piecesToMove = getPiecesByColor(color); 
    for (ChessPiece* piece : piecesToMove) {
        auto possibleMoves = piece->getPossibleMoves(*this); 
        for (const Move& currentMove : possibleMoves) {
            Board testBoard = *this;

            if (testBoard.makeMove(currentMove)) {
                if (!testBoard.isKingInCheck(color)) {
                    return false;
                }
            }
        }
    }

    return true;
}

void Board::print() const {
    std::cout << "  a b c d e f g h\n";
    std::cout << " +-----------------+\n";

    for (int j = 7; j >= 0; j--) {
        std::cout << j + 1 << "| ";
        for (int i = 0; i < 8; i++) {
            ChessPiece* piece = board[i][j].get();
            std::cout << (piece ? piece->getSymbol() : '.') << " ";
        }
        std::cout << "|\n";
    }
    std::cout << " +-----------------+\n";
}
