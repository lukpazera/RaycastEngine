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
	char getCell(int x, int y, int level);
    bool isWall(int x, int y);
	bool isWall(int x, int y, int level);
	void draw();

private:
    int _mapWidth;
    int _mapHeight;
	int _mapLevels;
    std::string _map;

};
