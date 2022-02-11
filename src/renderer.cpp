#include "renderer.hpp"

Renderer::Renderer() :
    _map(NULL),
	_player(NULL),
	_resX(320),
	_resY(240),
    _resMultiplier(4),
	_debugDrawing(false)
{
    _fov = 3.14159 / 2.0f; // 90 degrees FOV by default
    _maxTestingDepth = 16.0f;
    
	setLightAngle(3.14159 / 3.0);
    
	_debugRays.clear();
	_debugFOVPoints.clear();
}

Renderer::~Renderer()
{
}

void Renderer::setDebugDrawing(bool state)
{
	_debugDrawing = state;
}

void Renderer::setMap(Map *map)
{
    _map = map;
	_raycaster.setMap(*_map);
}

void Renderer::setPlayer(Player* player)
{
    _player = player;
}

void Renderer::setFOV(float fov)
{
	_fov = fov;
}

void Renderer::setRenderingDepth(int depth)
{
	_maxTestingDepth = depth;
}

void Renderer::setLightAngle(float angle)
{
	_lightAngle = angle;
	_lightDirection.x = sinf(_lightAngle);
	_lightDirection.y = cosf(_lightAngle);
}

void Renderer::setLightIntensity(float intensity)
{
	_lightIntensity = intensity;
}

void Renderer::increaseResolution()
{
    if (_resMultiplier == 1) { return; }
    _resMultiplier /= 2;
	onResolutionChanged();
}

void Renderer::decreaseResolution()
{
    if (_resMultiplier >= 16) { return; }
    _resMultiplier *= 2;
	onResolutionChanged();
}

// -------- Private methods

void Renderer::_calculateRenderSize()
{
    int screenWidth = ofGetWidth();
    int screenHeight = ofGetHeight();
    _resX = screenWidth / _resMultiplier;
    _resY = screenHeight / _resMultiplier;
}

void Renderer::_drawDebug()
{
	if (!_debugDrawing) { return; }
	
	float cellSize = float(ofGetHeight() / _map->getHeight());
	ofVec2f p1 = _player->getPosition() * cellSize;
	for (auto it = std::begin(_debugRays); it != std::end(_debugRays); ++it)
	{
		ofVec2f p2 = ((*it * cellSize) + p1);
		ofSetColor(255, 0, 64);
		ofDrawLine(p1, p2);
	}

	for (int i = 0; i < _debugFOVPoints.size(); i+=16)
	{
		ofVec2f p = p1 + (_debugFOVPoints.at(i) * cellSize * 1);
		ofSetColor(255, 128, 255);
		ofDrawCircle(p, 1);
	}
}
