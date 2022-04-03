#include "map.hpp"

Map::Map() :
	_mapWidth(16),
	_mapHeight(16),
	_mapLevels(2)
{    
	// ground level
    _map = "";
    _map += "$$$$$$$$########";
    _map += "$......$.......#";
    _map += "$.........####.#";
    _map += "$..$...$..#....#";
    _map += "$$$$.$$$..#....#";
    _map += "#.........######";
    _map += "#..$...........#";
    _map += "#..$..####.....#";
    _map += "#..$...........#";
    _map += "$$$$..$$..$$...#";
    _map += "#..............#";
    _map += "#....######....#";
    _map += "#....#....#....#";
    _map += "#......$$......#";
    _map += "#....#....#....#";
    _map += "################";

	// first level
	_map += "................";
	_map += "$..............#";
	_map += "$.........####.#";
	_map += "$..$......#....#";
	_map += "$$$$......#....#";
	_map += "..........######";
	_map += "...$...........#";
	_map += "...$..####.....#";
	_map += "...$...........#";
	_map += "$$$$..$$..$$...#";
	_map += "...............#";
	_map += ".....######....#";
	_map += ".....#....#....#";
	_map += ".......$$......#";
	_map += ".....#....#....#";
	_map += "################";
}

void Map::setSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
}

int Map::getWidth()
{
    return _mapWidth;
}

int Map::getHeight()
{
    return _mapHeight;
}

char Map::getCell(int x, int y, int level)
{
	level = max(level, 0);
	level = min(level, _mapLevels - 1);

	if (x < 0 || x >= _mapWidth) { return CELL_SPACE; }
	if (y < 0 || y >= _mapHeight) { return CELL_SPACE; }

	int levelOffset = level * _mapHeight * _mapWidth;
	int indexInMap = (y * _mapWidth) + x + levelOffset;
	return _map.at(indexInMap);
}

char Map::getCell(int x, int y)
{
	return getCell(x, y, 0);
    //int indexInMap = (y * _mapWidth) + x;
    //return _map.at(indexInMap);    
}

bool Map::isWall(int x, int y, int level)
{
    char cell = getCell(x, y, level); 
    return (cell == CELL_SPACE) ? false : true;
}

bool Map::isWall(int x, int y)
{
	return isWall(x, y, 0);
}

/*
 * Draws map on the screen, useful for debugging purposes.
 * Map fits the screen vertically.
 */
void Map::draw()
{
	int screenHeight = ofGetHeight();

	int blockSize = screenHeight / _mapHeight;

	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			char cell = getCell(x, y);
			if (cell == CELL_SPACE) { continue; }

			// draw cell
			ofSetColor(192, 192, 192);
			ofDrawRectangle(x * blockSize + 1, y * blockSize + 1, blockSize - 2, blockSize - 2);
		}
	}
}
