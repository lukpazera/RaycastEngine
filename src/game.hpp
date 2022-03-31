#pragma once

#include <string>
#include "ofMain.h"
#include "ofVec2f.h"
#include "map.hpp"
#include "player.hpp"
#include "cpuRenderer.hpp"
#include "hybridRenderer.hpp"

class Game
{
public:
	Game();

    void setup();
    void update();
    void draw();

	Player* getPlayer();
	HybridRenderer* getRenderer();
	bool debugDrawing;

private:
    Player _player;
    Map _map;
	HybridRenderer _renderer;

};
