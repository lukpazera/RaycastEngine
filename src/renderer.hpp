#pragma once

#include <string>
#include "ofMain.h"

#include "map.hpp"
#include "player.hpp"
#include "texture.hpp"

class Renderer
{
public:
    Renderer();

    void update();
    void draw();

    void setMap(Map* map);
    void setPlayer(Player* player);

    void increaseResolution();
    void decreaseResolution();

private:
    /* Calculates new render size based on resolution multiplier.
     */
    void _calculateRenderSize();
    
    /*
     * Reallocates render image buffer.
     */
    void _allocateRenderBuffer();
    
    // Resolution of the rendered image in pixels.
    int _resX;
    int _resY;
    
    // Resolution multiplier.
    // 1 means pixels will be 1x1 size.
    // 2 means pixels will be 2x2, etc.
    int _resMultiplier;
    
    ofImage _buffer;
    Map* _map;
    Player* _player;

    float _fov;
    float _maxTestingDepth;
    float _lightAngle;
    ofVec2f _lightDirection;
    
    ofImage _tex;
    ofImage _texMetal;
    Texture _tex1;
    
};
