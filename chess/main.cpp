#include <iostream>
#include <string>
#include <vector>
#include "Game.h"
#include "ChessTypes.h"

int main() {
    Game game;
    std::string reason;

    while (!game.isGameOver(reason)) {
        game.printBoard();
        Color turn = game.getCurrentTurn();

        if (turn == Color::WHITE) {
            std::cout << "Your turn (e.g., e2e4): ";
            std::string moveStr;
            std::cin >> moveStr;

            if (moveStr.length() != 4) {
                std::cout << "Invalid move format. Please use format like 'e2e4'.\n";
                continue;
            }

            Position from(moveStr.substr(0, 2));
            Position to(moveStr.substr(2, 4));

            if (!game.makeMove(Move(from, to))) {
                std::cout << "Invalid move. Try again.\n";
            }
        } else {
            std::cout << "Computer is thinking...\n";
            Move bestMove = game.findBestMove(4); // Глубина поиска 4 полухода
            game.makeMove(bestMove);
            std::cout << "Computer plays: " << bestMove.toString() << std::endl;
        }
    }

    game.printBoard();
    std::cout << "Game Over: " << reason << std::endl;

    return 0;
}