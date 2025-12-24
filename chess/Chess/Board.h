#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include "ChessTypes.h"

class ChessPiece;

class Board {
public:
    Board();
    ~Board();
    Board(const Board& other);
    Board& operator=(const Board& other);

    void clear();
    void addPiece(std::unique_ptr<ChessPiece> piece);
    void removePiece(const Position& position);
    ChessPiece* getPieceAt(const Position& position) const;

    bool makeMove(const Move& move);
    std::unique_ptr<ChessPiece> makeMoveWithCapture(const Move& move);
    void undoMove(const Move& move, std::unique_ptr<ChessPiece> capturedPiece, bool wasHasMoved);

    std::vector<ChessPiece*> getPiecesByColor(Color color) const;
    Position findKing(Color color) const;
    bool isPositionAttacked(const Position& position, Color attackingColor) const;
    bool isKingInCheck(Color color) const;
    bool isKingInCheckmate(Color color);
    
    std::vector<Move> getAllLegalMoves(Color color);
    void print() const;

private:
    std::unique_ptr<ChessPiece> board[8][8];
};

#endif 