#include <iostream>
#include <string>
#include <vector>
#include "Game.h"
#include "ChessTypes.h"

// Вспомогательная функция для парсинга символа превращения
PieceType parsePromotion(char p) {
    switch (std::tolower(p)) {
        case 'q': return PieceType::QUEEN;
        case 'r': return PieceType::ROOK;
        case 'b': return PieceType::BISHOP;
        case 'n': return PieceType::KNIGHT;
        default:  return PieceType::KING; // Означает "нет превращения" или неверный ввод
    }
}

int main() {
    Game game;
    game.loadConfig("config.txt"); // Загружаем конфигурацию
    std::string reason;

    while (!game.isGameOver(reason)) {
        game.printBoard();
        Color turn = game.getCurrentTurn();

        if (turn == Color::WHITE) {
            std::cout << "Your turn (e.g., e2e4 or e7e8q for promotion): ";
            std::string moveStr;
            std::cin >> moveStr;

            if (moveStr.length() < 4 || moveStr.length() > 5) {
                std::cout << "Invalid move format. Please use 'e2e4' or 'e7e8q'.\n";
                continue;
            }

            Position from(moveStr.substr(0, 2));
            Position to(moveStr.substr(2, 2)); // ИСПРАВЛЕНО: всегда читаем 2 символа для 'to'
            PieceType promoType = PieceType::KING; // По умолчанию нет превращения

            if (moveStr.length() == 5) {
                promoType = parsePromotion(moveStr[4]);
                if (promoType == PieceType::KING) {
                    std::cout << "Invalid promotion piece. Use q, r, b, or n.\n";
                    continue;
                }
            }

            if (!from.isValid() || !to.isValid()) {
                std::cout << "Invalid position in move string.\n";
                continue;
            }

            if (!game.makeMove(Move(from, to, promoType))) {
                std::cout << "Invalid move. Try again.\n";
            }

        } else {
            std::cout << "Computer is thinking...\n";
            Move bestMove = game.findBestMove(); // Используем глубину из конфига
            game.makeMove(bestMove);
            std::cout << "Computer plays: " << bestMove.toString() << std::endl;
        }
    }

    game.printBoard();
    std::cout << "Game Over: " << reason << std::endl;

    return 0;
}