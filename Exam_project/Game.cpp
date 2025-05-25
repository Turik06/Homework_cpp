#include "Game.h"
#include "ChessPieces.h"
#include <fstream>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>

const int Game::MATE_SCORE;
const int Game::INFINITY_SCORE;

Game::Game() {}

bool Game::loadFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) return false;
    board.clear();
    std::string line;
    bool readingWhite = false, readingBlack = false;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        if (line.find("White:") != std::string::npos) {
            readingWhite = true; readingBlack = false; continue;
        }
        if (line.find("Black:") != std::string::npos) {
            readingWhite = false; readingBlack = true; continue;
        }
        if (readingWhite || readingBlack) {
            if (line.size() < 2) continue;
            char t = line[0];
            std::string posStr = line.substr(1);
            Position pos(posStr);
            if (!pos.isValid()) continue;
            char sym = readingWhite ? std::toupper(t) : std::tolower(t);
            auto p = createPiece(sym, pos);
            if (p) board.addPiece(std::move(p));
        }
    }
    return true;
}

std::unique_ptr<ChessPiece> Game::createPiece(char sym, const Position& pos) const {
    bool isWhite = std::isupper(sym);
    char s = std::toupper(sym);
    switch (s) {
    case 'K': return std::make_unique<King>(isWhite ? Color::WHITE : Color::BLACK, pos);
    case 'Q': return std::make_unique<Queen>(isWhite ? Color::WHITE : Color::BLACK, pos);
    case 'R': return std::make_unique<Rook>(isWhite ? Color::WHITE : Color::BLACK, pos);
    case 'B': return std::make_unique<Bishop>(isWhite ? Color::WHITE : Color::BLACK, pos);
    case 'N': return std::make_unique<Knight>(isWhite ? Color::WHITE : Color::BLACK, pos);
    case 'P': return std::make_unique<Pawn>(isWhite ? Color::WHITE : Color::BLACK, pos);
    default: return nullptr;
    }
}

void Game::printBoard() const {
    board.print();
}

bool Game::isCurrentPositionMate() const {
    if (board.isKingInCheckmate(Color::BLACK)) return true;
    if (board.isKingInCheckmate(Color::WHITE)) return true;
    return false;
}

std::vector<Move> Game::generateOrderedMoves(const Board& currentBoard, Color color) const {
    std::vector<Move> allMoves;
    allMoves = getAllLegalMoves(currentBoard, color);

    std::sort(allMoves.begin(), allMoves.end(), [&](const Move& a, const Move& b) {
        int scoreA = 0;
        int scoreB = 0;
        Board boardA = currentBoard;
        if (boardA.makeMove(a) && !boardA.isKingInCheck(color) && boardA.isKingInCheck(color == Color::WHITE ? Color::BLACK : Color::WHITE)) scoreA += 10;
        ChessPiece* capturedA = currentBoard.getPieceAt(a.to);
        if (capturedA) scoreA += getPieceValue(capturedA->getType());

        Board boardB = currentBoard;
        if (boardB.makeMove(b) && !boardB.isKingInCheck(color) && boardB.isKingInCheck(color == Color::WHITE ? Color::BLACK : Color::WHITE)) scoreB += 10;
        ChessPiece* capturedB = currentBoard.getPieceAt(b.to);
        if (capturedB) scoreB += getPieceValue(capturedB->getType());

        return scoreA > scoreB;
        });
    return allMoves;
}

int Game::evaluateMove(const Board& currentBoard, const Move& move, Color color) const {
    int score = 0;
    Board testBoard = currentBoard;
    if (!testBoard.makeMove(move)) return -INFINITY_SCORE;
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (testBoard.isKingInCheckmate(opponent)) return MATE_SCORE;
    if (testBoard.isKingInCheck(opponent)) score += 5000;
    ChessPiece* captured = currentBoard.getPieceAt(move.to);
    if (captured) {
        score += getPieceValue(captured->getType()) * 100;
    }
    if (testBoard.isKingInCheck(color)) {
        score = -MATE_SCORE;
    }
    return score;
}

int Game::getPieceValue(PieceType type) const {
    switch (type) {
    case PieceType::QUEEN: return 900;
    case PieceType::ROOK: return 500;
    case PieceType::BISHOP: return 330;
    case PieceType::KNIGHT: return 320;
    case PieceType::PAWN: return 100;
    case PieceType::KING: return 20000;
    default: return 0;
    }
}

std::vector<Move> Game::getAllLegalMoves(const Board& currentBoard, Color color) const {
    std::vector<Move> legalMoves;
    auto pieces = currentBoard.getPiecesByColor(color);
    for (ChessPiece* piece : pieces) {
        auto possibleMoves = piece->getPossibleMoves(currentBoard);
        for (const Move& move : possibleMoves) {
            Board testBoard = currentBoard;
            if (testBoard.makeMove(move)) {
                if (!testBoard.isKingInCheck(color)) {
                    legalMoves.push_back(move);
                }
            }
        }
    }
    return legalMoves;
}

std::string Game::movesToString(const std::vector<Move>& moves) const {
    std::stringstream ss;
    for (size_t i = 0; i < moves.size(); ++i) {
        ss << moves[i].toString();
        if (i < moves.size() - 1) {
            ss << " ";
        }
    }
    return ss.str();
}

int Game::evaluateBoard(const Board& currentBoard, Color playerToMove) const {
    Color opponent = (playerToMove == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (currentBoard.isKingInCheckmate(opponent)) {
        return MATE_SCORE;
    }
    if (currentBoard.isKingInCheckmate(playerToMove)) {
        return -MATE_SCORE;
    }
    return 0;
}

int Game::alphaBetaSearch(Board currentBoard, int depthInPlies, int alpha, int beta, Color playerToMove,
    std::vector<Move>& bestMoveSequenceForThisNode) const {
    bestMoveSequenceForThisNode.clear();

    if (depthInPlies == 0) {
        return evaluateBoard(currentBoard, playerToMove);
    }

    std::vector<Move> legalMoves = generateOrderedMoves(currentBoard, playerToMove);

    if (legalMoves.empty()) {
        if (currentBoard.isKingInCheck(playerToMove)) {
            return -MATE_SCORE;
        }
        else {
            return 0;
        }
    }

    Color opponent = (playerToMove == Color::WHITE) ? Color::BLACK : Color::WHITE;
    int bestScore = -INFINITY_SCORE;

    for (const Move& move : legalMoves) {
        Board nextBoard = currentBoard;
        nextBoard.makeMove(move);

        std::vector<Move> sequenceFromChild;
        int score = -alphaBetaSearch(nextBoard, depthInPlies - 1, -beta, -alpha, opponent, sequenceFromChild);

        if (score > bestScore) {
            bestScore = score;
            bestMoveSequenceForThisNode.clear();
            bestMoveSequenceForThisNode.push_back(move);
            bestMoveSequenceForThisNode.insert(bestMoveSequenceForThisNode.end(), sequenceFromChild.begin(), sequenceFromChild.end());
        }

        if (bestScore > alpha) {
            alpha = bestScore;
        }

        if (alpha >= beta) {
            break;
        }
    }
    return bestScore;
}

std::set<std::string> Game::findForcedMate_AlphaBeta(int numFullMoves) const {
    std::set<std::string> mateSequences;
    if (numFullMoves <= 0) return mateSequences;

    Color startingPlayer = Color::WHITE;
    Color opponentPlayer = Color::BLACK;
    int targetDepthInPlies = numFullMoves * 2 - 1;

    auto firstLegalMoves = getAllLegalMoves(this->board, startingPlayer);
    std::cout << "Проверка ходов белых:\n";
   
    for (const Move& firstMove : firstLegalMoves) {
        std::cout << "- " << firstMove.toString() << "\n";
        Board boardAfterFirstMove = this->board;
        boardAfterFirstMove.makeMove(firstMove);

     

        if (numFullMoves == 1) {
            if (boardAfterFirstMove.isKingInCheckmate(opponentPlayer)) {
                mateSequences.insert(firstMove.toString());
            }
            continue;
        }

        std::vector<Move> bestContinuation;
        int scoreForOpponent = alphaBetaSearch(
            boardAfterFirstMove,
            targetDepthInPlies - 1,
            -INFINITY_SCORE,
            INFINITY_SCORE,
            opponentPlayer,
            bestContinuation
        );

        if (scoreForOpponent == -MATE_SCORE) {
            std::vector<Move> fullSequence;
            fullSequence.push_back(firstMove);
            fullSequence.insert(fullSequence.end(), bestContinuation.begin(), bestContinuation.end());

            if (fullSequence.size() == static_cast<size_t>(targetDepthInPlies)) {
                mateSequences.insert(movesToString(fullSequence));
            }
        }
    }
    return mateSequences;
}



std::map<int, std::set<std::string>> Game::findAllMateSequences(int maxDepthFullMoves) const {
    std::map<int, std::set<std::string>> results;

    for (int N = 1; N <= maxDepthFullMoves; ++N) {
        
        results[N] = findForcedMate_AlphaBeta(N);

      
    }

    return results;
}


