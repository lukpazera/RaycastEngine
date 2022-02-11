#include "map.hpp"

Map::Map()
{
    _mapWidth = 16;
    _mapHeight = 16;
    
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

char Map::getCell(int x, int y)
{
    int indexInMap = (y * _mapWidth) + x;
    return _map.at(indexInMap);    
}

bool Map::isWall(int x, int y)
{
    char cell = getCell(x, y); 
    return (cell == CELL_SPACE) ? false : true;
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
