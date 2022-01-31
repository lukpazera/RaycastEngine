#pragma once

#include <string>
#include "ofMain.h"


const static char CELL_SPACE = '.';

class Map
{
public:
    Map();

    void setSize(int width, int height);
    int getWidth();
    int getHeight();
    char getCell(int x, int y);
    bool isWall(int x, int y);
	void draw();

private:
    int _mapWidth;
    int _mapHeight;
    std::string _map;

};
