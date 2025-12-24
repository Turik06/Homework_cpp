#include "Game.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"

Game::Game() {}

bool Game::loadFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) return false;
    board.clear();

    std::string line;
    bool readingWhite = false, readingBlack = false;
    int whiteCount = 0, blackCount = 0;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        if (line.find("White:") != std::string::npos) {
            std::istringstream iss(line);
            std::string tmp; iss >> tmp >> whiteCount;
            readingWhite = true; readingBlack = false; continue;
        }
        if (line.find("Black:") != std::string::npos) {
            std::istringstream iss(line);
            std::string tmp; iss >> tmp >> blackCount;
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

std::unique_ptr<ChessPiece> Game::createPiece(char sym, const Position& pos) {
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

bool Game::canWhiteCheckmateInOneMove() const { return board.canCheckmateInOneMove(Color::WHITE); }
bool Game::canBlackCheckmateInOneMove() const { return board.canCheckmateInOneMove(Color::BLACK); }
bool Game::canWhiteCheckmateInTwoMoves() const { return board.canCheckmateInTwoMoves(Color::WHITE); }
bool Game::canBlackCheckmateInTwoMoves() const { return board.canCheckmateInTwoMoves(Color::BLACK); }
bool Game::canWhiteCheckmateInNMoves(int n) const {
    return mateAnalyzer.canMateInNMoves(board, Color::WHITE, n);
}

bool Game::canBlackCheckmateInNMoves(int n) const {
    return mateAnalyzer.canMateInNMoves(board, Color::BLACK, n);
}

std::vector<Move> Game::getWhiteMatingSequence(int n) const {
    return mateAnalyzer.getMatingSequence(board, Color::WHITE, n);
}

std::vector<Move> Game::getBlackMatingSequence(int n) const {
    return mateAnalyzer.getMatingSequence(board, Color::BLACK, n);
}

void Game::printBoard() const { board.print(); }
