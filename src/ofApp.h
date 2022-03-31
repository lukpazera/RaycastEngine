#pragma once

#include <string>
#include "ofMain.h"
#include "ofVec2f.h"
#include "game.hpp"
#include "map.hpp"
#include "player.hpp"
#include "cpuRenderer.hpp"
#include "hybridRenderer.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
	Game _game;
    
	bool _debugDrawing;
	float _moveSpeed;
	bool _moveForward;
	bool _moveBackward;
	bool _moveLeft;
	bool _moveRight;
	bool _mouseWrapped;
};
