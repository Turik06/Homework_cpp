#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// Структура для описания типа муравьев (из конфига)
struct AntType {
    std::string name = "Default";
    int count = 10;
    double alpha = 1.0;
    double beta = 2.0;
};

class Config {
public:
    // Глобальные настройки
    int num_iterations;        
    double evaporation_rate;   
    std::string input_file;    
    std::string output_file;   
    
    // Вектор типов муравьев
    std::vector<AntType> ant_types;

    // Значения по умолчанию
    Config() : num_iterations(100), evaporation_rate(0.1), 
               input_file("test.txt"), output_file("output.txt") {}

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: Не удалось открыть файл конфигурации " << filename << std::endl;
            return false;
        }

        std::string line;
        AntType* currentAnt = nullptr; // Указатель на текущий настраиваемый тип

        while (std::getline(file, line)) {
            // Удаляем комментарии и лишние пробелы
            size_t commentPos = line.find('#');
            if (commentPos != std::string::npos) {
                line = line.substr(0, commentPos);
            }
            line = trim(line);
            if (line.empty()) continue;

            // Если встретили заголовок новой секции
            if (line == "[Ant]") {
                ant_types.push_back(AntType()); // Добавляем нового муравья в список
                currentAnt = &ant_types.back(); // Переключаем указатель на него
                continue;
            }

            // Парсим строку вида "key = value"
            std::istringstream iss(line);
            std::string key, equals, valueStr;
            iss >> key >> equals >> valueStr;

            if (equals != "=") continue;

            // Если мы внутри секции [Ant], настраиваем конкретного муравья
            if (currentAnt != nullptr) {
                if (key == "name") currentAnt->name = valueStr;
                else if (key == "count") currentAnt->count = std::stoi(valueStr);
                else if (key == "alpha") currentAnt->alpha = std::stod(valueStr);
                else if (key == "beta") currentAnt->beta = std::stod(valueStr);
            } 
            // Иначе настраиваем глобальные параметры
            else {
                if (key == "num_iterations") num_iterations = std::stoi(valueStr);
                else if (key == "evaporation_rate") evaporation_rate = std::stod(valueStr);
                else if (key == "input_file") input_file = valueStr;
                else if (key == "output_file") output_file = valueStr;
            }
        }
        file.close();
        
        // Если в конфиге не было муравьев, добавим одного стандартного
        if (ant_types.empty()) {
            ant_types.push_back(AntType());
        }
        
        return true;
    }

    void print() const {
        std::cout << "=== Конфигурация ===" << std::endl;
        std::cout << "Входной файл: " << input_file << std::endl;
        std::cout << "Выходной файл: " << output_file << std::endl;
        std::cout << "Итерации: " << num_iterations << std::endl;
        std::cout << "Испарение: " << evaporation_rate << std::endl;
        std::cout << "--- Типы муравьев ---" << std::endl;
        for (const auto& ant : ant_types) {
            std::cout << "Тип: " << ant.name 
                      << " | Кол-во: " << ant.count 
                      << " | Alpha: " << ant.alpha 
                      << " | Beta: " << ant.beta << std::endl;
        }
        std::cout << "====================" << std::endl;
    }

private:
    // Вспомогательная функция для удаления пробелов по краям
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (std::string::npos == first) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }
};

#endif // CONFIG_H