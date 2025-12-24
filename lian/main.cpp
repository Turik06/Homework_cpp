#include "lian.h"
#include "settings.h"
#include <fstream>
#include <iomanip> 
#include <cmath>

int main(int argc, char* argv[]) {
    string configname = "config.txt";
    
    if (argc > 1) {
        configname = argv[1];
    }
    
    cout << "Using config file: " << configname << "\n";

    Config config(configname);

    if (config.filename.empty()) {
        cout << "Error: Config file is empty or invalid.\n";
        return -1;
    }

    Map algMap(config.filename);

    if (algMap.width == 0 || algMap.height == 0) {
        cout << "Error: Failed to load map.\n";
        return -2;
    }
    
    Cell* start = algMap.getCell(config.startX,config.startY);
    Cell* end = algMap.getCell(config.endX,config.endY);

    if (!start || !end || start->obstacle || end->obstacle) {
        cout << "Error: Invalid start/end points.\n";
        return -3;
    }

    LIAN lian(algMap, start, end, config.step, config.angle);
    vector<Cell*> path = lian.findPath();

    if (!path.empty()) {
        cout << "Path found! Points: " << path.size() << "\n";
        
      
        ofstream outfile(config.pathOutFile);
        if (outfile.is_open()) {
            for (Cell* c : path) {
                outfile << c->x << " " << c->y << "\n";
            }
            outfile.close();
            cout << "Simple path saved to '" << config.pathOutFile << "'\n";
        }

 
        ofstream robotFile(config.robotOutFile);
        if (robotFile.is_open()) {
            robotFile << "X Y Angle G H F\n"; 
            
            robotFile << fixed << setprecision(2);
            
            for (size_t i = 0; i < path.size(); ++i) {
                float angle = 0.0f;
                float dx = 0, dy = 0;

                if (i < path.size() - 1) {
                    dx = (float)(path[i+1]->x - path[i]->x);
                    dy = (float)(path[i+1]->y - path[i]->y);
                } else if (i > 0) {
                    dx = (float)(path[i]->x - path[i-1]->x);
                    dy = (float)(path[i]->y - path[i-1]->y);
                }

                if (dx != 0 || dy != 0) {
                    angle = atan2(dy, dx) * 180.0f / M_PI;
                }

                robotFile << path[i]->x << " " 
                          << path[i]->y << " " 
                          << angle << " "
                          << path[i]->g << " " 
                          << path[i]->h << " " 
                          << path[i]->f << "\n";
            }
            robotFile.close();
            cout << "Detailed path saved to '" << config.robotOutFile << "'\n";
        }
    }
    else
        cout << "Path not found" << '\n';

    return 0;
}