#include "Game.h"
#include "ChessPieces.h"
#include <iostream>
#include <limits>
#include <algorithm>

Game::Game() {
    setupNewGame();
}

void Game::setupNewGame() {
    board.clear();
    turn = Color::WHITE;

    // Расстановка белых фигур
    board.addPiece(std::make_unique<Rook>(Color::WHITE, Position("a1")));
    board.addPiece(std::make_unique<Knight>(Color::WHITE, Position("b1")));
    board.addPiece(std::make_unique<Bishop>(Color::WHITE, Position("c1")));
    board.addPiece(std::make_unique<Queen>(Color::WHITE, Position("d1")));
    board.addPiece(std::make_unique<King>(Color::WHITE, Position("e1")));
    board.addPiece(std::make_unique<Bishop>(Color::WHITE, Position("f1")));
    board.addPiece(std::make_unique<Knight>(Color::WHITE, Position("g1")));
    board.addPiece(std::make_unique<Rook>(Color::WHITE, Position("h1")));
    for (int i = 0; i < 8; ++i) {
        board.addPiece(std::make_unique<Pawn>(Color::WHITE, Position(i, 1)));
    }

    // Расстановка черных фигур
    board.addPiece(std::make_unique<Rook>(Color::BLACK, Position("a8")));
    board.addPiece(std::make_unique<Knight>(Color::BLACK, Position("b8")));
    board.addPiece(std::make_unique<Bishop>(Color::BLACK, Position("c8")));
    board.addPiece(std::make_unique<Queen>(Color::BLACK, Position("d8")));
    board.addPiece(std::make_unique<King>(Color::BLACK, Position("e8")));
    board.addPiece(std::make_unique<Bishop>(Color::BLACK, Position("f8")));
    board.addPiece(std::make_unique<Knight>(Color::BLACK, Position("g8")));
    board.addPiece(std::make_unique<Rook>(Color::BLACK, Position("h8")));
    for (int i = 0; i < 8; ++i) {
        board.addPiece(std::make_unique<Pawn>(Color::BLACK, Position(i, 6)));
    }
}

bool Game::makeMove(const Move& move) {
    auto legalMoves = getAllLegalMoves(board, turn);
    bool isLegal = false;
    for (const auto& legalMove : legalMoves) {
        if (legalMove.from == move.from && legalMove.to == move.to) {
            // Для пешек нужно также проверить тип превращения, если он есть
            if (legalMove.promotionType == PieceType::KING || legalMove.promotionType == move.promotionType) {
                 isLegal = true;
                 break;
            }
        }
    }

    if (!isLegal) {
        return false;
    }

    ChessPiece* piece = board.getPieceAt(move.from);
    if (!piece) return false;

    board.makeMove(move);

    // --- Обработка превращения пешки ---
    if (piece->getType() == PieceType::PAWN && move.promotionType != PieceType::KING) {
        board.removePiece(move.to); // Удаляем пешку
        switch (move.promotionType) { // Добавляем новую фигуру
            case PieceType::QUEEN:
                board.addPiece(std::make_unique<Queen>(turn, move.to));
                break;
            case PieceType::ROOK:
                board.addPiece(std::make_unique<Rook>(turn, move.to));
                break;
            case PieceType::BISHOP:
                board.addPiece(std::make_unique<Bishop>(turn, move.to));
                break;
            case PieceType::KNIGHT:
                board.addPiece(std::make_unique<Knight>(turn, move.to));
                break;
            default: break;
        }
    }

    turn = (turn == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return true;
}

void Game::printBoard() const {
    board.print();
}

bool Game::isGameOver(std::string& reason) {
    auto legalMoves = getAllLegalMoves(board, turn);

    if (legalMoves.empty()) {
        if (board.isKingInCheck(turn)) {
            reason = (turn == Color::WHITE) ? "Checkmate! Black wins." : "Checkmate! White wins.";
        } else {
            reason = "Stalemate! It's a draw.";
        }
        return true;
    }
    return false;
}

int Game::getPieceValue(PieceType type) const {
    switch (type) {
        case PieceType::PAWN:   return 100;
        case PieceType::KNIGHT: return 320;
        case PieceType::BISHOP: return 330;
        case PieceType::ROOK:   return 500;
        case PieceType::QUEEN:  return 900;
        case PieceType::KING:   return 20000;
        default: return 0;
    }
}

int Game::evaluateBoard(const Board& currentBoard) const {
    int totalScore = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            ChessPiece* piece = currentBoard.getPieceAt(Position(i, j));
            if (piece) {
                if (piece->getColor() == Color::WHITE) {
                    totalScore += getPieceValue(piece->getType());
                } else {
                    totalScore -= getPieceValue(piece->getType());
                }
            }
        }
    }
    return totalScore;
}

int Game::alphaBeta(Board currentBoard, int depth, int alpha, int beta, bool maximizingPlayer) const {
    if (depth == 0) {
        return evaluateBoard(currentBoard);
    }

    Color playerColor = maximizingPlayer ? Color::WHITE : Color::BLACK;
    auto legalMoves = getAllLegalMoves(currentBoard, playerColor);

    if (legalMoves.empty()) {
        if (currentBoard.isKingInCheck(playerColor)) {
            return maximizingPlayer ? -MATE_SCORE : MATE_SCORE;
        }
        return 0; // Stalemate
    }

    if (maximizingPlayer) {
        int maxEval = -INFINITY_SCORE;
        for (const auto& move : legalMoves) {
            Board nextBoard = currentBoard;
            nextBoard.makeMove(move);
            int eval = alphaBeta(nextBoard, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = INFINITY_SCORE;
        for (const auto& move : legalMoves) {
            Board nextBoard = currentBoard;
            nextBoard.makeMove(move);
            int eval = alphaBeta(nextBoard, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

Move Game::findBestMove(int depth) {
    Move bestMove;
    int bestValue = (turn == Color::WHITE) ? -INFINITY_SCORE : INFINITY_SCORE;
    
    auto legalMoves = getAllLegalMoves(board, turn);

    for (const auto& move : legalMoves) {
        Board nextBoard = board;
        nextBoard.makeMove(move);

        int boardValue = alphaBeta(nextBoard, depth - 1, -INFINITY_SCORE, INFINITY_SCORE, turn == Color::BLACK);

        if (turn == Color::WHITE) {
            if (boardValue > bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        } else {
            if (boardValue < bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        }
    }
    return bestMove;
}

std::vector<Move> Game::getAllLegalMoves(const Board& currentBoard, Color color) const {
    std::vector<Move> legalMoves;
    auto pieces = currentBoard.getPiecesByColor(color);

    for (ChessPiece* piece : pieces) {
        auto possibleMoves = piece->getPossibleMoves(currentBoard);
        for (const Move& move : possibleMoves) {
            Board testBoard = currentBoard;
            testBoard.makeMove(move);
            if (!testBoard.isKingInCheck(color)) {
                legalMoves.push_back(move);
            }
        }
    }
    return legalMoves;
}