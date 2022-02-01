#pragma once

#include <string>
#include "ofMain.h"
#include "ofVec2f.h"
#include "map.hpp"
#include "player.hpp"
#include "renderer.hpp"

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
    Player _player;
    Map _map;
    Renderer _renderer;
    
    ofImage _tex;
    ofImage _texMetal;
    ofImage _bg;
    
    double _slowestFrame;
	bool _debugDrawing;
};
