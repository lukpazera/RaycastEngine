#include "map.hpp"

Map::Map()
{
    _mapWidth = 16;
    _mapHeight = 16;
    
    _map = "";
    _map += "################";
    _map += "#..............#";
    _map += "#.........##...#";
    _map += "#..........#...#";
    _map += "#..$...........#";
    _map += "#..$..##.......#";
    _map += "#......##$.....#";
    _map += "#..............#";
    _map += "#..$...........#";
    _map += "$$$$......$$...#";
    _map += "#..............#";
    _map += "#....######....#";
    _map += "#......#.......#";
    _map += "#..............#";
    _map += "#..............#";
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
