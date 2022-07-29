#pragma once

#include <string>
#include "ofMain.h"
#include "texture.hpp"

const static char CELL_SPACE = '.';

class MapCell
{
public:
	MapCell();
	MapCell(char ident, bool isWall);
	MapCell(char ident, bool isWall, Texture &tex);

	bool isWall();
	char ident;

private:
	Texture *_tex;
	std::string _textureIdent;
	bool _isWall;
};


class Map
{
public:
    Map();

    void setSize(int width, int height);
    int getWidth();
    int getHeight();
    MapCell getCell(int x, int y);
	MapCell getCell(int x, int y, int level);
    bool isWall(int x, int y);
	bool isWall(int x, int y, int level);
	void draw();

private:
    int _mapWidth;
    int _mapHeight;
	int _mapLevels;
    std::string _map;
	std::map<char, MapCell> _cells;
	std::map<char, char> _test;
	std::vector<MapCell> _mapReady;
};
