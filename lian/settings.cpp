#include "settings.h"

Config::Config(string configname) {
    ifstream file(configname);

    if (!file.is_open()){
        cout << "Error: Failed to read config file" << '\n';
        return;
    }

    string dummy; 

    file >> filename; 
    getline(file, dummy); 

    file >> startX >> startY;
    getline(file, dummy);

    file >> endX >> endY;
    getline(file, dummy);

    file >> step >> angle;
    getline(file, dummy);

    file >> pathOutFile;
    if (pathOutFile.empty()) pathOutFile = "path_result.txt"; // Дефолт на всякий случай
    getline(file, dummy);

    file >> robotOutFile;
    if (robotOutFile.empty()) robotOutFile = "robot_path.txt"; // Дефолт на всякий случай
    getline(file, dummy);

    file.close();
}