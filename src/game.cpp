#include "game.hpp"

//--------------------------------------------------------------
Game::Game() :
	debugDrawing(false),
	_player{ Player() },
	_map{ Map() }
	//_renderer{ HybridRenderer() }
{
}

Player* Game::getPlayer()
{
	return &_player;
}

HybridRenderer* Game::getRenderer()
{
	return &_renderer;
}

void Game::setup()
{
    _player.setMap(&_map);
    
	// Init needs to be called manually here.
	// This needs to be refactored so renderer class calls onInit()
	// by itself when renderer instance is created via factory.
	_renderer.onInit();

    _renderer.setMap(&_map);
    _renderer.setPlayer(&_player);
	_renderer.setResolution(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void Game::update()
{

}

//--------------------------------------------------------------
void Game::draw()
{    
    _renderer.draw();
    
	if (debugDrawing)
	{
		_map.draw();
		_player.draw();
	}

	int fpsval = int(ofGetFrameRate());
	std::string fps = ofToString(fpsval);
	ofDrawBitmapStringHighlight(fps, 40, 40);

	if (debugDrawing)
	{
		ofSetColor(40, 255, 40);
		ofDrawRectangle(20, 70, 4 * fpsval, 20);
	}
}

