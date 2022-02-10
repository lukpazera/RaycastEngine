#pragma once

#include <string>
#include "ofMain.h"

#include "map.hpp"
#include "player.hpp"
#include "texture.hpp"
#include "ddaRaycaster.h"

class Renderer
{
public:
    Renderer();
	~Renderer();

    void update();
    void draw();

    void setMap(Map* map);
    void setPlayer(Player* player);

    void increaseResolution();
    void decreaseResolution();

	void setDebugDrawing(bool state);

private:
    /* Calculates new render size based on resolution multiplier.
     */
    void _calculateRenderSize();
    
    /*
     * Reallocates render image buffer.
     */
    void _allocateRenderBuffer();
    
	void _drawDebug();

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
	DDARaycaster _raycaster;

    float _fov;
    float _maxTestingDepth;
    float _lightAngle;
    ofVec2f _lightDirection;
    
    ofImage _tex;
    ofImage _texMetal;
    Texture _tex1;

	bool _debugDrawing;
	vector<ofVec2f> _debugRays;
	vector<ofVec2f> _debugFOVPoints;
};