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

	virtual void update() {};
	virtual void draw() {};
	virtual void onInit() {};
	virtual void onResolutionChanged() {};

	void setFOV(float fov); // in radians
	void setRenderingDepth(int depth); // in map cells
    void setMap(Map* map);
    void setPlayer(Player* player);
	void setLightAngle(float angle);
	void setLightIntensity(float intensity);

    void increaseResolution();
    void decreaseResolution();

	void setDebugDrawing(bool state);

private:
    /* Calculates new render size based on resolution multiplier.
     */
    void _calculateRenderSize();
    
	void _drawDebug();

    // Resolution of the rendered image in pixels.
    int _resX;
    int _resY;
    
    // Resolution multiplier.
    // 1 means pixels will be 1x1 size.
    // 2 means pixels will be 2x2, etc.
    int _resMultiplier;
    
    Map* _map;
    Player* _player;
	DDARaycaster _raycaster;

    float _fov;
    float _maxTestingDepth;
    float _lightAngle;
	float _lightIntensity;
    ofVec2f _lightDirection;

	bool _debugDrawing;
	vector<ofVec2f> _debugRays;
	vector<ofVec2f> _debugFOVPoints;
};