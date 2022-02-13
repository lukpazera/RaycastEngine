#include "renderer.hpp"

Renderer::Renderer() :
    _map(NULL),
	_player(NULL),
	debugDrawing(false),
	_maxTestingDepth(16.0f)
{
	setLightAngle(3.14159 / 3.0);
    
	_updateRenderInfo();

	debugRays.clear();
	debugFOVPoints.clear();

	onInit();
}

Renderer::~Renderer()
{
}

void Renderer::draw()
{
	// We'll store rays in vector for later drawing on screen.
	if (debugDrawing)
	{
		debugRays.clear();
		debugFOVPoints.clear();
	}

	onDraw();

	if (debugDrawing) { drawDebug(); }
}

Map* Renderer::getMap() const
{
	return _map;
}

void Renderer::setMap(Map *map)
{
    _map = map;
	raycaster.setMap(*map);
}

Player* Renderer::getPlayer() const
{
	return _player;
}

void Renderer::setPlayer(Player* player)
{
    _player = player;
}

void Renderer::setResolution(int x, int y)
{
	renderInfo.outputResolutionX = x;
	renderInfo.outputResolutionY = y;
	_updateRenderInfo();
	onResolutionChanged();
}

void Renderer::setFOV(float fov)
{
	renderInfo.fov = fov;
	_updateRenderInfo();
}

void Renderer::setRenderingDepth(int depth)
{
	_maxTestingDepth = depth;
}

void Renderer::setLightAngle(float angle)
{
	lightInfo.angle = angle;
	lightInfo.direction.x = sinf(angle);
	lightInfo.direction.y = cosf(angle);
}

void Renderer::setLightIntensity(float intensity)
{
	lightInfo.intensity = intensity;
}

void Renderer::increaseResolution()
{
    if (renderInfo.resolutionMultiplier == 1) { return; }
    renderInfo.resolutionMultiplier /= 2;
	_updateRenderInfo();
	onResolutionChanged();
}

void Renderer::decreaseResolution()
{
    if (renderInfo.resolutionMultiplier >= 16) { return; }
    renderInfo.resolutionMultiplier *= 2;
	_updateRenderInfo();
	onResolutionChanged();
}

// -------- Private methods

void Renderer::_updateRenderInfo()
{
	renderInfo.renderResolutionX = renderInfo.outputResolutionX / renderInfo.resolutionMultiplier;
	renderInfo.renderResolutionY = renderInfo.outputResolutionY / renderInfo.resolutionMultiplier;

	renderInfo.aspectRatio = float(renderInfo.outputResolutionX) / float(renderInfo.outputResolutionY);

	renderInfo.fovWidth = tan(renderInfo.fov / 2.0) * 2.0; // basic trigonometry
	renderInfo.fovWidthStep = renderInfo.fovWidth / float(renderInfo.renderResolutionX);

	renderInfo.hfovCells = renderInfo.fovWidth;
	renderInfo.vfovCells = renderInfo.fovWidth / renderInfo.aspectRatio;
	renderInfo.cellPixelSize = renderInfo.renderResolutionY / renderInfo.vfovCells;
}

void Renderer::drawDebug()
{
	if (!debugDrawing) { return; }
	
	float cellSize = float(ofGetHeight() / _map->getHeight());
	ofVec2f p1 = _player->getPosition() * cellSize;
	for (auto it = std::begin(debugRays); it != std::end(debugRays); ++it)
	{
		ofVec2f p2 = ((*it * cellSize) + p1);
		ofSetColor(255, 0, 64);
		ofDrawLine(p1, p2);
	}

	for (int i = 0; i < debugFOVPoints.size(); i+=16)
	{
		ofVec2f p = p1 + (debugFOVPoints.at(i) * cellSize * 1);
		ofSetColor(255, 128, 255);
		ofDrawCircle(p, 1);
	}
}
