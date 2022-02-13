#pragma once

#include <string>
#include "ofMain.h"

#include "map.hpp"
#include "player.hpp"
#include "texture.hpp"
#include "ddaRaycaster.h"

struct RenderInfo
{
	RenderInfo() : 
		outputResolutionX(960),
		outputResolutionY(540),
		resolutionMultiplier(2),
		renderResolutionX(480),
		renderResolutionY(270),
		fov(3.14159 / 2.0f) // 90 degrees FOV by default) 
		{};
	int outputResolutionX;
	int outputResolutionY;
	int resolutionMultiplier;
	int renderResolutionX;
	int renderResolutionY;
	float aspectRatio;
	float fov;
	float fovWidth;
	float fovWidthStep;
	float hfovCells; // how many cells fit fov horizontally when viewed from distance of 1 
	float vfovCells; // how many cells fit fov vertically when viewed from distance of 1
	float cellPixelSize; // how big is 1 cell in pixels when viewed from distance of 1
};

struct LightInfo
{
	LightInfo() : angle(0), direction(ofVec2f(1, 0)), intensity(1.0) {};

	float angle;
	ofVec2f direction;
	float intensity;
};

class Renderer
{
public:
    Renderer();
	~Renderer();

	virtual void update() {};
	virtual void onDraw() {};
	virtual void onInit() {};
	virtual void onResolutionChanged() {};

	void draw();
	void setResolution(int x, int y);
	void setFOV(float fov); // in radians
	void setRenderingDepth(int depth); // in map cells
	Map* getMap() const;
    void setMap(Map* map);
	Player* getPlayer() const;
	void setPlayer(Player* player);
	void setLightAngle(float angle);
	void setLightIntensity(float intensity);

    void increaseResolution();
    void decreaseResolution();

	DDARaycaster raycaster;
	RenderInfo renderInfo; // read only, do not change this directly!
	LightInfo lightInfo;

	bool debugDrawing;
	vector<ofVec2f> debugRays;
	vector<ofVec2f> debugFOVPoints;
	void drawDebug();

private:
	void _updateRenderInfo();

	Map* _map;
	Player* _player;

    float _maxTestingDepth;
    //float _lightAngle;
	//float _lightIntensity;
    //ofVec2f _lightDirection;

	// Note that vision line length says how many cells fit into view horizontally.
	// We can use this to know the height of a cell in pixels from any distance.
	//float aspectRatio = float(_resX) / float(_resY);
	//float vcells = visionLineLength / aspectRatio; // how many cells fit view vertically when viewed from distance of 1
	//float dist1Height = float(_resY) * vcells; // height of a wall in pixels when viewed from distance of 1.
};