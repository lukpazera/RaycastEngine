#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    _slowestFrame = 0;
	_debugDrawing = false;

    _fov = 3.14159 / 2.5f;
    _maxTestingDepth = 16.0f;
    
    _player.setMap(&_map);
    
    _renderer.setMap(&_map);
    _renderer.setPlayer(&_player);

    _lightAngle = 3.14159 / 3.0;
    _lightDirection.x = sinf(_lightAngle);
    _lightDirection.y = cosf(_lightAngle);
    
    _tex.load("TexConcrete1024.png");
    _texMetal.load("TexMetal1024.png");
    _bg.load("Background.png");
    
    _renderWidth = ofGetWidth();
    _renderHeight = ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::update()
{
    _renderer.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{    
    _renderer.draw();

    double lastFrameTime = ofGetLastFrameTime();
    if (lastFrameTime > _slowestFrame)
    {
        _slowestFrame = lastFrameTime;
    }
    
	if (_debugDrawing)
	{
		_map.draw();
	}

    int fpsval = (int)(1.0 / lastFrameTime);
    std::string fps = std::to_string(fpsval);
    ofDrawBitmapStringHighlight(fps, 40, 40);
    ofSetColor(40, 255, 40);
    ofDrawRectangle(20, 70, 4 * fpsval, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd')
    {
        _player.rotate(0.2f); // * ofGetLastFrameTime());
    }
    else if (key == 'a')
    {
        _player.rotate(-0.2f); // * ofGetLastFrameTime());
    }
    else if (key == 'w')
    {
        _player.move(0.6f); // * ofGetLastFrameTime());
    }
    else if (key == 's')
    {
        _player.move(-0.6f);// * ofGetLastFrameTime());
    }
    
    // Increase/descrease resolution
    else if (key == ',')
    {
        _renderer.decreaseResolution();
    }
    else if (key == '.')
    {
        _renderer.increaseResolution();
    }

	// Toggle debug drawing
	else if (key == '1')
	{
		_debugDrawing = !_debugDrawing;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
