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
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (other.board[i][j]) {
                board[i][j] = other.board[i][j]->clone();
            } else {
                board[i][j] = nullptr;
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
            board[i][j].reset();
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
        board[position.x][position.y].reset();
    }
}

ChessPiece* Board::getPieceAt(const Position& position) const {
    if (position.isValid()) {
        return board[position.x][position.y].get();
    }
    return nullptr;
}

std::unique_ptr<ChessPiece> Board::makeMoveWithCapture(const Move& move) {
    ChessPiece* piece = getPieceAt(move.from);
    if (!piece) return nullptr;

    std::unique_ptr<ChessPiece> captured = nullptr;

    if (piece->getType() == PieceType::KING && std::abs(move.to.x - move.from.x) == 2) {
        if (move.to.x > move.from.x) {
            board[6][move.from.y] = std::move(board[4][move.from.y]);
            board[6][move.from.y]->setPosition(Position(6, move.from.y));
            board[6][move.from.y]->setHasMoved(true);
            
            board[5][move.from.y] = std::move(board[7][move.from.y]);
            board[5][move.from.y]->setPosition(Position(5, move.from.y));
            board[5][move.from.y]->setHasMoved(true);
        } else {
            board[2][move.from.y] = std::move(board[4][move.from.y]);
            board[2][move.from.y]->setPosition(Position(2, move.from.y));
            board[2][move.from.y]->setHasMoved(true);
            
            board[3][move.from.y] = std::move(board[0][move.from.y]);
            board[3][move.from.y]->setPosition(Position(3, move.from.y));
            board[3][move.from.y]->setHasMoved(true);
        }
        return nullptr;
    }

    if (board[move.to.x][move.to.y]) {
        captured = std::move(board[move.to.x][move.to.y]);
    }

    board[move.to.x][move.to.y] = std::move(board[move.from.x][move.from.y]);
    piece->setPosition(move.to);
    piece->setHasMoved(true);

    return captured;
}

void Board::undoMove(const Move& move, std::unique_ptr<ChessPiece> capturedPiece, bool wasHasMoved) {
    ChessPiece* piece = getPieceAt(move.to);
    if (!piece) return;

    if (piece->getType() == PieceType::KING && std::abs(move.to.x - move.from.x) == 2) {
        if (move.to.x > move.from.x) {
            board[4][move.from.y] = std::move(board[6][move.from.y]);
            board[4][move.from.y]->setPosition(Position(4, move.from.y));
            board[4][move.from.y]->setHasMoved(wasHasMoved);

            board[7][move.from.y] = std::move(board[5][move.from.y]);
            board[7][move.from.y]->setPosition(Position(7, move.from.y));
            board[7][move.from.y]->setHasMoved(false);
        } else {
            board[4][move.from.y] = std::move(board[2][move.from.y]);
            board[4][move.from.y]->setPosition(Position(4, move.from.y));
            board[4][move.from.y]->setHasMoved(wasHasMoved);

            board[0][move.from.y] = std::move(board[3][move.from.y]);
            board[0][move.from.y]->setPosition(Position(0, move.from.y));
            board[0][move.from.y]->setHasMoved(false);
        }
        return;
    }

    board[move.from.x][move.from.y] = std::move(board[move.to.x][move.to.y]);
    piece->setPosition(move.from);
    piece->setHasMoved(wasHasMoved);

    if (capturedPiece) {
        capturedPiece->setPosition(move.to);
        board[move.to.x][move.to.y] = std::move(capturedPiece);
    }
}

bool Board::makeMove(const Move& move) {
    makeMoveWithCapture(move);
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
            if (piece && piece->getColor() == color && piece->getType() == PieceType::KING) {
                return piece->getPosition();
            }
        }
    }
    return Position(-1, -1);
}

bool Board::isPositionAttacked(const Position& position, Color attackingColor) const {
    auto pieces = getPiecesByColor(attackingColor);
    for (auto piece : pieces) {
        if (piece->isValidMove(*this, Move(piece->getPosition(), position))) {
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

std::vector<Move> Board::getAllLegalMoves(Color color) {
    std::vector<Move> legalMoves;
    legalMoves.reserve(50); 

    auto pieces = getPiecesByColor(color);

    for (ChessPiece* piece : pieces) {
        auto possibleMoves = piece->getPossibleMoves(*this);
        bool originalHasMoved = piece->getHasMoved();

        for (const Move& move : possibleMoves) {
            std::unique_ptr<ChessPiece> captured = makeMoveWithCapture(move);
            
            if (!isKingInCheck(color)) {
                legalMoves.push_back(move);
            }

            undoMove(move, std::move(captured), originalHasMoved);
        }
    }
    return legalMoves;
}

bool Board::isKingInCheckmate(Color color) {
    if (!isKingInCheck(color)) {
        return false;
    }
    return getAllLegalMoves(color).empty();
}

void Board::print() const {
    std::cout << "    a   b   c   d   e   f   g   h\n";
    std::cout << "  +---+---+---+---+---+---+---+---+\n";
    for (int j = 7; j >= 0; j--) {
        std::cout << j + 1 << " | ";
        for (int i = 0; i < 8; i++) {
            ChessPiece* piece = board[i][j].get();
            std::cout << (piece ? piece->getSymbol() : '.') << " | ";
        }
        std::cout << j + 1 << "\n";
        std::cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "    a   b   c   d   e   f   g   h\n";
}