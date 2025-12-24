#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Config {
public:
    string filename;    
    int startX, startY;
    int endX, endY;
    float step, angle;
    
    string pathOutFile;  
    string robotOutFile;  
    Config(string configname);
};
#endif