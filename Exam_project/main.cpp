#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include "Game.h"

bool isMeaninglessSequence(const std::string& sequence) {
    std::vector<std::string> moves;
    std::stringstream ss(sequence);
    std::string move;
    while (ss >> move) {
        moves.push_back(move);
    }

    if (moves.size() >= 3) {
        for (size_t i = 0; i < moves.size() - 2; i++) {
            if (moves[i] == moves[i + 2]) {
                return true;
            }
        }
    }

    return false;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " input.txt\n";
        return 1;
    }

    Game game;
    if (!game.loadFromFile(argv[1])) {
        std::cerr << "Ошибка загрузки файла: " << argv[1] << "\n";
        return 1;
    }

    std::cout << "=== ШАХМАТНАЯ ПОЗИЦИЯ ===\n";
    game.printBoard();


    if (game.isCurrentPositionMate()) {
        std::cout << "Позиция уже является матом!\n";
        std::ofstream file("output.txt");
        if (file.is_open()) {
            file << "# Мат уже поставлен - игра окончена\n";
        }
        return 0;
    }

    std::cout << "\n=== АНАЛИЗ МАТА В 3 ХОДА (Alpha-Beta поиск) ===\n";


    auto matesByDepth = game.findAllMateSequences(3);

    bool foundAny = false;
    std::ofstream outFile("output.txt");

    for (int depth = 1; depth <= 3; depth++) {
        if (matesByDepth.find(depth) != matesByDepth.end() && !matesByDepth[depth].empty()) {

            std::string depthText;
            if (depth == 1) depthText = "мат в 1 ход";
            else if (depth == 2) depthText = "мат в 2 хода";
            else depthText = "мат в 3 хода";

            std::vector<std::string> validSequences;
            for (const auto& sequence : matesByDepth[depth]) {
                if (!isMeaninglessSequence(sequence)) {
                    validSequences.push_back(sequence);
                }
            }

            if (!validSequences.empty()) {
                foundAny = true;
                std::cout << depthText << ": ";
                if (outFile.is_open()) {
                    outFile << depthText << ": ";
                }

                bool first = true;
                for (const auto& sequence : validSequences) {
                    if (!first) {
                        std::cout << ", ";
                        if (outFile.is_open()) outFile << ", ";
                    }
                    std::cout << sequence;
                    if (outFile.is_open()) outFile << sequence;
                    first = false;
                }
                std::cout << "\n";
                if (outFile.is_open()) outFile << "\n";
            }
        }
    }


    if (!foundAny) {
        std::cout << "Мат в 3 хода не найден.\n";
        if (outFile.is_open()) {
            outFile << "# Матовая последовательность для белых не найдена\n";
        }
    }
    else {
        std::cout << "Результаты сохранены в output.txt\n";
    }

    return 0;
}
