#include <iostream>
#include <string>
#include "Game.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " input.txt\n";
        return 1;
    }
    std::string fname = argv[1];
    Game g;
    if (!g.loadFromFile(fname)) {
        std::cerr << "Failed to load file\n"; return 1;
    }
    g.printBoard();

    std::cout << "Analysis:\n";

    if (g.canWhiteCheckmateInOneMove()) std::cout << "White mate in 1\n";
    else if (g.canWhiteCheckmateInTwoMoves()) std::cout << "White mate in 2\n";
    else if (g.canWhiteCheckmateInNMoves(3)) std::cout << "White mate in 3\n";
    else if (g.canWhiteCheckmateInNMoves(4)) std::cout << "White mate in 4\n";

    if (g.canBlackCheckmateInOneMove()) std::cout << "Black mate in 1\n";
    else if (g.canBlackCheckmateInTwoMoves()) std::cout << "Black mate in 2\n";
    else if (g.canBlackCheckmateInNMoves(3)) std::cout << "Black mate in 3\n";
    else if (g.canBlackCheckmateInNMoves(4)) std::cout << "Black mate in 4\n";

    return 0;
}
